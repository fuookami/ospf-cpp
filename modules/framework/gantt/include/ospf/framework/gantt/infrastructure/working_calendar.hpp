#pragma once
/// Gantt infrastructure working_calendar /// 1:1 对应 Rust gantt/infrastructure/working_calendar.rs
/// ActualTime + ValidTimes + WorkingCalendar

#include <ospf/framework/gantt/infrastructure/time_range.hpp>
#include <ospf/framework/gantt/infrastructure/time_window.hpp>
#include <ospf/framework/gantt/infrastructure/duration_range.hpp>

#include <algorithm>
#include <cstddef>
#include <optional>
#include <vector>

namespace ospf::framework::gantt {

    // ============================================================================
    // 实际时间结果 / Actual time result
    // 对应 Rust ActualTime
    // ============================================================================

    /// 实际时间结果 / Actual time result
    /// 包含实际时间范围、工作时间段、休息时间段、连接时间段
    struct ActualTime {
        /// 实际时间范围 / Actual time range
        TimeRange time;
        /// 工作时间段 / Working time ranges
        std::vector<TimeRange> working_times;
        /// 休息时间段 / Break time ranges
        std::vector<TimeRange> break_times;
        /// 连接时间段 / Connection time ranges
        std::vector<TimeRange> connection_times;

        /// 工作时长（小时） / Working duration (hours)
        [[nodiscard]] double working_duration() const noexcept {
            double sum = 0.0;
            for (const auto& t : working_times) sum += t.duration();
            return sum;
        }

        /// 休息时长（小时） / Break duration (hours)
        [[nodiscard]] double break_duration() const noexcept {
            double sum = 0.0;
            for (const auto& t : break_times) sum += t.duration();
            return sum;
        }

        /// 连接时长（小时） / Connection duration (hours)
        [[nodiscard]] double connection_duration() const noexcept {
            double sum = 0.0;
            for (const auto& t : connection_times) sum += t.duration();
            return sum;
        }
    };

    // ============================================================================
    // 有效时间结果 / Valid times result
    // 对应 Rust ValidTimes
    // ============================================================================

    /// 有效时间结果 / Valid times result
    struct ValidTimes {
        /// 有效时间段 / Valid time ranges
        std::vector<TimeRange> times;
        /// 休息时间段 / Break time ranges
        std::vector<TimeRange> break_times;
        /// 连接时间段 / Connection time ranges
        std::vector<TimeRange> connection_times;

        /// 有效时长（小时） / Valid duration (hours)
        [[nodiscard]] double duration() const noexcept {
            double sum = 0.0;
            for (const auto& t : times) sum += t.duration();
            return sum;
        }
    };

    // ============================================================================
    // 工作日历 / Working calendar
    // 对应 Rust WorkingCalendar<V>
    // ============================================================================

    /// 工作日历 / Working calendar
    /// 管理不可用时间段，计算实际工作时间和有效时间范围
    struct WorkingCalendar {
        /// 时间窗口 / Time window
        TimeWindow window;
        /// 不可用时间段 / Unavailable time ranges
        std::vector<TimeRange> unavailable_times;

        /// 创建 / Create
        [[nodiscard]] static WorkingCalendar create(
            TimeWindow win, std::vector<TimeRange> unavail = {})
        {
            return WorkingCalendar{std::move(win), std::move(unavail)};
        }

        /// 计算实际时间（排除不可用时间段） / Compute actual time (excluding unavailable)
        /// 对应 Rust WorkingCalendar::actual_time_at
        [[nodiscard]] double actual_time_at(
            double instant,
            const std::vector<TimeRange>& extra_unavailable = {},
            std::optional<double> before_connection = std::nullopt,
            std::optional<double> after_connection = std::nullopt) const
        {
            // 如果 instant 在不可用时间段内，跳到该段结束 + after_connection
            double t = instant;
            for (const auto& unavail : unavailable_times) {
                if (t >= unavail.start && t < unavail.end) {
                    t = unavail.end;
                    if (after_connection) t += *after_connection;
                    break;
                }
            }
            for (const auto& extra : extra_unavailable) {
                if (t >= extra.start && t < extra.end) {
                    t = extra.end;
                    if (after_connection) t += *after_connection;
                    break;
                }
            }
            return t;
        }

        /// 计算实际时间范围（扩展补偿不可用段，保持总工作时长 == 原始范围时长）
        /// 对应 Rust WorkingCalendar::actual_time_range
        /// 核心语义：从 range.start 开始累积工作时间，遇到不可用段则跳过，直到累计工作 == range.duration()
        /// 例如：range[8,12)=4h + 不可用[10,11) → 工作段 [8,10)+[11,13) = 4h（窗口扩展到 13:00）
        /// 特例：整个范围被不可用段覆盖 → 工作时长 0h
        [[nodiscard]] ActualTime actual_time_range(
            const TimeRange& range,
            const std::vector<TimeRange>& extra_unavailable = {},
            std::optional<double> before_connection = std::nullopt,
            std::optional<double> after_connection = std::nullopt) const
        {
            // 合并所有不可用时间段
            std::vector<TimeRange> all_unavail;
            for (const auto& u : unavailable_times) all_unavail.push_back(u);
            for (const auto& u : extra_unavailable) all_unavail.push_back(u);
            std::sort(all_unavail.begin(), all_unavail.end(),
                [](const TimeRange& a, const TimeRange& b) { return a.start < b.start; });

            // 特例：范围完全被不可用段覆盖 → 返回 0h 工作时间（不扩展）
            std::vector<TimeRange> subtracted = {range};
            for (const auto& u : all_unavail) {
                std::vector<TimeRange> next;
                for (const auto& r : subtracted) {
                    auto diff = r.difference(u);
                    next.insert(next.end(), diff.begin(), diff.end());
                }
                subtracted = std::move(next);
            }
            double subtracted_total = 0.0;
            for (const auto& r : subtracted) subtracted_total += r.duration();

            std::vector<TimeRange> working_times;
            if (subtracted_total < 1e-10) {
                // 完全覆盖 → 0h
            } else {
                // 有可用时间 → 扩展补偿：从 range.start 开始累积工作，遇到不可用段跳过，保持总工作量 == range.duration()
                double target_work = range.duration();
                double cursor = range.start;
                double work_done = 0.0;
                double window_end = window.window.end;

                while (work_done < target_work - 1e-10 && cursor < window_end + 1e-10) {
                    bool blocked = false;
                    for (const auto& u : all_unavail) {
                        if (cursor >= u.start && cursor < u.end) {
                            cursor = u.end;
                            blocked = true;
                            break;
                        }
                    }
                    if (blocked) continue;

                    double seg_end = cursor + (target_work - work_done);
                    seg_end = std::min(seg_end, window_end);
                    for (const auto& u : all_unavail) {
                        if (u.start > cursor && u.start < seg_end) {
                            seg_end = u.start;
                        }
                    }

                    double seg_len = seg_end - cursor;
                    if (seg_len <= 1e-10) {
                        // 段长为 0（已到窗口边界或被完全覆盖）→ 退出循环
                        break;
                    }
                    working_times.push_back({cursor, seg_end});
                    cursor = seg_end;
                    work_done += seg_len;
                }
            }

            ActualTime result;
            result.time = range;
            result.working_times = working_times;
            result.break_times = {};
            result.connection_times = {};
            return result;
        }
    };

}  // namespace ospf::framework::gantt
