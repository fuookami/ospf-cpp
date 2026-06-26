#pragma once
/// Gantt infrastructure local_date_offset /// 1:1 对应 Rust gantt/infrastructure/local_date_offset.rs
/// LocalDateOffset — 跨日边界逻辑日期偏移

#include <cstddef>

namespace ospf::framework::gantt {

    /// 本地日期偏移 / Local date offset
    /// 对应 Rust LocalDateOffset
    /// 定义了逻辑日期切换点：当日内时间早于 offset 时，逻辑日期为日历日期前一天。
    struct LocalDateOffset {
        /// 日偏移时间（小时，0-24）/ Daily offset time (hours, 0-24)
        double offset_hours = 0.0;

        /// 创建 / Create
        /// 对应 Rust LocalDateOffset::new
        [[nodiscard]] static LocalDateOffset create(double offset_h) {
            return LocalDateOffset{offset_h};
        }

        /// 默认值（0:00，无偏移） / Default (0:00, no offset)
        [[nodiscard]] static LocalDateOffset default_offset() {
            return LocalDateOffset{0.0};
        }

        /// 计算逻辑日期（day 序号） / Calculate logical date (day index)
        /// 对应 Rust LocalDateOffset::date
        /// 当 time_hours < offset_hours 时，逻辑日期为 day - 1
        [[nodiscard]] int logical_day(int calendar_day, double time_hours) const {
            return (time_hours < offset_hours) ? calendar_day - 1 : calendar_day;
        }
    };

}  // namespace ospf::framework::gantt
