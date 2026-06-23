#pragma once

/// 范围边界 / Range bounds
/// 模仿 Rust 的 std::ops::RangeBounds。

#include <optional>

namespace ospf::base {

    /// 范围边界 / Range bounds (inclusive start, exclusive end)
    template<typename T>
    struct RangeBounds {
        T start;
        T end;

        [[nodiscard]] bool contains(const T& value) const noexcept {
            return value >= start && value < end;
        }

        [[nodiscard]] bool is_empty() const noexcept {
            return start >= end;
        }
    };

    /// 闭合范围 / Closed range (inclusive both ends)
    template<typename T>
    struct ClosedRange {
        T start;
        T end;

        [[nodiscard]] bool contains(const T& value) const noexcept {
            return value >= start && value <= end;
        }
    };

}  // namespace ospf::base
