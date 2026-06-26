#pragma once
/// Gantt infrastructure time_range /// 1:1 对应 Rust gantt/infrastructure/time_range.rs
/// TimeRange — 半开区间 [start, end) 时间范围
/// 使用 double 表示时间（小时级精度），对齐 Rust TimeRange 语义

#include <algorithm>
#include <cstddef>
#include <vector>

namespace ospf::framework::gantt {

    // ============================================================================
    // 时间范围 / Time range
    // 对应 Rust TimeRange（半开区间 [start, end)）
    // ============================================================================

    /// 时间范围 / Time range
    /// 对应 Rust TimeRange（半开区间 [start, end)）
    /// 使用 double 表示时间（小时级精度），对齐 Rust TimeRange 语义
    struct TimeRange {
        /// 开始时间（小时） / Start time (hours)
        double start = 0.0;
        /// 结束时间（小时） / End time (hours)
        double end = 0.0;

        /// 创建 / Create
        [[nodiscard]] static TimeRange create(double s, double e) {
            return TimeRange{s, e};
        }

        /// 持续时间 / Duration
        [[nodiscard]] double duration() const noexcept {
            return end - start;
        }

        /// 是否为空 / Whether empty
        [[nodiscard]] bool is_empty() const noexcept {
            return start >= end;
        }

        /// 是否相交 / Whether intersects
        [[nodiscard]] bool intersects(const TimeRange& other) const noexcept {
            return start < other.end && other.start < end;
        }

        /// 是否包含 / Whether contains
        [[nodiscard]] bool contains(double t) const noexcept {
            return t >= start && t < end;
        }

        /// 差集（单区间） / Difference (single)
        /// 返回 base - other 的结果（可能 0、1 或 2 个区间）
        /// 对应 Rust TimeRange::difference
        [[nodiscard]] std::vector<TimeRange> difference(const TimeRange& other) const {
            std::vector<TimeRange> result;
            if (!intersects(other)) {
                // 不相交，保持原样
                result.push_back(*this);
                return result;
            }
            // 相交，分割
            if (start < other.start) {
                result.push_back({start, other.start});
            }
            if (end > other.end) {
                result.push_back({other.end, end});
            }
            return result;
        }

        /// 差集（多区间） / Difference with many
        /// 对应 Rust TimeRange::difference_with_many
        [[nodiscard]] std::vector<TimeRange> difference_with_many(
            const std::vector<TimeRange>& others) const
        {
            std::vector<TimeRange> remaining = {*this};
            for (const auto& other : others) {
                std::vector<TimeRange> new_remaining;
                for (const auto& r : remaining) {
                    auto diff = r.difference(other);
                    new_remaining.insert(new_remaining.end(), diff.begin(), diff.end());
                }
                remaining = std::move(new_remaining);
            }
            return remaining;
        }

        [[nodiscard]] bool operator==(const TimeRange& other) const noexcept {
            return start == other.start && end == other.end;
        }
    };

}  // namespace ospf::framework::gantt
