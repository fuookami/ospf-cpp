#pragma once
/// 倒数 / Reciprocal /// 1:1 对应 Rust operator/reciprocal.rs
namespace ospf::math::op {
    template<typename T> [[nodiscard]] T reciprocal(T v) { return T{1} / v; }
}
