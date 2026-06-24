#pragma once
/// 坐标轴 / Coordinate axis /// 1:1 对应 Rust geometry/axis.rs
#include <cstddef>
namespace ospf::math::geometry {
    enum class Axis : std::uint8_t { X = 0, Y = 1, Z = 2 };
    [[nodiscard]] constexpr Axis other_2d(Axis a) noexcept {
        return a == Axis::X ? Axis::Y : Axis::X;
    }
}
