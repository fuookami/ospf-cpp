#pragma once
/// Gantt infrastructure time_window /// 1:1 对应 Rust gantt/infrastructure/time_window.rs
/// DurationUnit + TimeWindow

#include <ospf/framework/gantt/infrastructure/time_range.hpp>

#include <algorithm>
#include <cmath>
#include <optional>

namespace ospf::framework::gantt {

    // ============================================================================
    // 持续时间单位 / Duration unit
    // 对应 Rust DurationUnit
    // ============================================================================

    /// 持续时间单位 / Duration unit
    /// 对应 Rust DurationUnit（Seconds/Minutes/Hours）
    enum class DurationUnit {
        Seconds,
        Minutes,
        Hours
    };

    /// 从 DurationUnit 创建 Duration（小时） / Create duration (hours) from DurationUnit
    /// 对应 Rust DurationUnit::from_value
    [[nodiscard]] inline double duration_from_value(DurationUnit unit, double value) noexcept {
        switch (unit) {
            case DurationUnit::Seconds: return value / 3600.0;
            case DurationUnit::Minutes: return value / 60.0;
            case DurationUnit::Hours:   return value;
        }
        return value;
    }

    // ============================================================================
    // 时间窗口 / Time window
    // 对应 Rust TimeWindow<V>
    // ============================================================================

    /// 时间窗口 / Time window
    /// 对应 Rust TimeWindow<V>
    struct TimeWindow {
        /// 基础时间范围 / Underlying time range
        TimeRange window;
        /// 是否连续 / Whether continuous
        bool continues = true;
        /// 持续时间单位 / Duration unit
        DurationUnit duration_unit = DurationUnit::Hours;
        /// 日期偏移量（小时） / Date offset (hours)
        double date_offset_hours = 0.0;
        /// 时间间隔（小时） / Time interval (hours)
        double interval_hours = 1.0;

        /// 创建小时级时间窗口 / Create hours-level time window
        /// 对应 Rust TimeWindow::hours
        [[nodiscard]] static TimeWindow hours(
            TimeRange win, double date_offset, bool cont, double interval)
        {
            return TimeWindow{win, cont, DurationUnit::Hours, date_offset, interval};
        }

        /// 创建分钟级时间窗口 / Create minutes-level time window
        /// 对应 Rust TimeWindow::minutes
        [[nodiscard]] static TimeWindow minutes(
            TimeRange win, double date_offset, bool cont, double interval)
        {
            return TimeWindow{win, cont, DurationUnit::Minutes, date_offset, interval};
        }

        /// 持续时间值（小时）/ Duration value (hours)
        /// 对应 Rust TimeWindow::duration
        [[nodiscard]] double duration() const noexcept {
            return window.duration();
        }

        /// 将持续时间转换为 solver 值 / Convert duration to solver value
        /// 对应 Rust TimeWindow::value_of_duration
        [[nodiscard]] double value_of_duration(double duration_hours) const {
            switch (duration_unit) {
                case DurationUnit::Seconds: return duration_hours * 3600.0;
                case DurationUnit::Minutes: return duration_hours * 60.0;
                case DurationUnit::Hours:   return duration_hours;
            }
            return duration_hours;
        }

        /// 将时间点转换为 solver 值 / Convert instant to solver value
        /// 对应 Rust TimeWindow::value_of_instant
        [[nodiscard]] double value_of_instant(double instant_hours) const {
            double adjusted = instant_hours - window.start - date_offset_hours;
            return value_of_duration(adjusted);
        }

        /// 从 solver 值转换为持续时间（小时）/ Convert solver value to duration (hours)
        /// 对应 Rust TimeWindow::duration_of
        [[nodiscard]] double duration_of(double value) const {
            switch (duration_unit) {
                case DurationUnit::Seconds: return value / 3600.0;
                case DurationUnit::Minutes: return value / 60.0;
                case DurationUnit::Hours:   return value;
            }
            return value;
        }
    };

}  // namespace ospf::framework::gantt
