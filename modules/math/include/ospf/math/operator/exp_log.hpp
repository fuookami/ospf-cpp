#pragma once
/// 指数对数 / Exp and log /// 1:1 对应 Rust operator/exp_log.rs
#include <cmath>
namespace ospf::math::op {
    template<typename T> [[nodiscard]] T exp(T v) { return std::exp(v); }
    template<typename T> [[nodiscard]] T ln(T v) { return std::log(v); }
    template<typename T> [[nodiscard]] T log10(T v) { return std::log10(v); }
    template<typename T> [[nodiscard]] T log2(T v) { return std::log2(v); }
}
