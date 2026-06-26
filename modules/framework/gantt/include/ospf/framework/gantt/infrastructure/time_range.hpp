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

        /// 是否包含时刻 / Whether contains instant (半开区间：含 start，不含 end)
        /// 对应 Rust TimeRange::contains_instant
        [[nodiscard]] bool contains_instant(double t) const noexcept {
            return t >= start && t < end;
        }

        /// 交集 / Intersection
        /// 对应 Rust TimeRange::intersection
        [[nodiscard]] std::optional<TimeRange> intersection(const TimeRange& other) const noexcept {
            double s = std::max(start, other.start);
            double e = std::min(end, other.end);
            if (s < e) return TimeRange{s, e};
            return std::nullopt;
        }

        /// 是否连续（当前 range 的 end == other 的 start）/ Whether continuous before
        /// 对应 Rust TimeRange::continuous_before
        [[nodiscard]] bool continuous_before(const TimeRange& other) const noexcept {
            return end == other.start;
        }

        /// 是否连续（当前 range 的 start == other 的 end）/ Whether continuous after
        /// 对应 Rust TimeRange::continuous_after
        [[nodiscard]] bool continuous_after(const TimeRange& other) const noexcept {
            return start == other.end;
        }

        /// 是否连续（任一方向）/ Whether continuous with
        /// 对应 Rust TimeRange::continuous_with
        [[nodiscard]] bool continuous_with(const TimeRange& other) const noexcept {
            return continuous_before(other) || continuous_after(other);
        }

        /// 在指定点处分割 / Split at specified points
        /// 对应 Rust TimeRange::split_at
        [[nodiscard]] std::vector<TimeRange> split_at(const std::vector<double>& points) const {
            std::vector<double> sorted_points;
            for (double p : points) {
                if (p > start && p < end) sorted_points.push_back(p);
            }
            std::sort(sorted_points.begin(), sorted_points.end());

            std::vector<TimeRange> result;
            double cur = start;
            for (double p : sorted_points) {
                result.push_back({cur, p});
                cur = p;
            }
            result.push_back({cur, end});
            return result;
        }

        /// 平移 / Shift
        /// 对应 Rust TimeRange::shift
        [[nodiscard]] TimeRange shift(double offset) const noexcept {
            return {start + offset, end + offset};
        }

        /// 创建空范围 / Create empty range
        /// 对应 Rust TimeRange::empty
        [[nodiscard]] static TimeRange empty() {
            return {0.0, 0.0};
        }

        /// 创建全范围 / Create full range
        /// 对应 Rust TimeRange::full
        [[nodiscard]] static TimeRange full() {
            return {-1e18, 1e18};
        }

        [[nodiscard]] bool operator==(const TimeRange& other) const noexcept {
            return start == other.start && end == other.end;
        }
    };

    // ============================================================================
    // 辅助函数 / Helper functions
    // 对应 Rust find / find_from / front_at / back_at / merge
    // ============================================================================

    /// 在日历中查找与范围相交的区间 / Find intersecting ranges in calendar
    /// 对应 Rust find
    [[nodiscard]] inline std::vector<TimeRange> find(
        const std::vector<TimeRange>& calendar,
        const TimeRange& range)
    {
        std::vector<TimeRange> result;
        for (const auto& entry : calendar) {
            if (entry.intersects(range)) {
                result.push_back(entry);
            }
        }
        return result;
    }

    /// 从时间点开始查找 / Find from time point
    /// 对应 Rust find_from
    [[nodiscard]] inline std::vector<TimeRange> find_from(
        const std::vector<TimeRange>& calendar,
        double from)
    {
        std::vector<TimeRange> result;
        for (const auto& entry : calendar) {
            if (entry.end > from) {
                result.push_back(entry);
            }
        }
        return result;
    }

    /// 第 i 个间隔区间的前面区间 / Front at gap index
    /// 对应 Rust front_at
    [[nodiscard]] inline std::optional<TimeRange> front_at(
        const std::vector<TimeRange>& ranges,
        std::size_t gap_index)
    {
        if (gap_index >= ranges.size() - 1) return std::nullopt;
        return TimeRange{ranges[gap_index].end, ranges[gap_index + 1].start};
    }

    /// 第 i 个间隔区间的后面区间 / Back at gap index
    /// 对应 Rust back_at
    [[nodiscard]] inline std::optional<TimeRange> back_at(
        const std::vector<TimeRange>& ranges,
        std::size_t gap_index)
    {
        if (gap_index >= ranges.size() - 1) return std::nullopt;
        return TimeRange{ranges[gap_index].end, ranges[gap_index + 1].start};
    }

    /// 合并重叠/相邻区间 / Merge overlapping/adjacent ranges
    /// 对应 Rust merge
    [[nodiscard]] inline std::vector<TimeRange> merge(std::vector<TimeRange> ranges) {
        if (ranges.empty()) return {};
        std::sort(ranges.begin(), ranges.end(),
            [](const TimeRange& a, const TimeRange& b) { return a.start < b.start; });
        std::vector<TimeRange> result;
        result.push_back(ranges[0]);
        for (std::size_t i = 1; i < ranges.size(); ++i) {
            if (ranges[i].start <= result.back().end) {
                result.back().end = std::max(result.back().end, ranges[i].end);
            } else {
                result.push_back(ranges[i]);
            }
        }
        return result;
    }

}  // namespace ospf::framework::gantt
