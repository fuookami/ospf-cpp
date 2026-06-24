#pragma once
/// GenesioTesi 混沌系统 / GenesioTesi chaotic system
/// 1:1 对应 Rust chaotic/genesio_tesi.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct GenesioTesiParams {
        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            // 简化实现：返回零导数（占位）
            // Simplified: returns zero derivatives (placeholder)
            return {S{0}, S{0}, S{0}};
        }
    };
    static constexpr char kGenesioTesiName[] = "genesio_tesi";
    using GenesioTesiSystem = ChaoticSystem3D<GenesioTesiParams, kGenesioTesiName>;
}
