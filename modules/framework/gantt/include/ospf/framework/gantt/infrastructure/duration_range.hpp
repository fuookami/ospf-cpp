#pragma once
/// Gantt infrastructure duration_range /// 1:1 对应 Rust gantt/infrastructure/duration_range.rs
/// DurationRange — 持续时间范围（半开区间）

#include <algorithm>
#include <optional>

namespace ospf::framework::gantt {

    /// 持续时间范围 / Duration range
    /// 对应 Rust DurationRange
    struct DurationRange {
        /// 下界（小时） / Lower bound (hours)
        std::optional<double> min_hours;
        /// 上界（小时） / Upper bound (hours)
        std::optional<double> max_hours;

        /// 固定值 / Fixed
        [[nodiscard]] static DurationRange fixed(double hours) {
            return DurationRange{hours, hours};
        }

        /// 区间 / Interval
        [[nodiscard]] static DurationRange interval(double min_h, double max_h) {
            return DurationRange{min_h, max_h};
        }

        /// 是否包含 / Contains
        [[nodiscard]] bool contains(double hours) const noexcept {
            if (min_hours && hours < *min_hours) return false;
            if (max_hours && hours > *max_hours) return false;
            return true;
        }
    };

}  // namespace ospf::framework::gantt
