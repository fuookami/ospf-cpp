#pragma once
/// LogisticMap 标量映射 / Logistic scalar map
/// 1:1 对应 Rust chaotic/logistic.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct LogisticMapParams {
        template<typename S = double>
        [[nodiscard]] S map(S x) const { return x; }
    };
    static constexpr char kLogisticMapName[] = "logistic_map";
    using LogisticMap = ScalarMap<LogisticMapParams, kLogisticMapName>;
}
