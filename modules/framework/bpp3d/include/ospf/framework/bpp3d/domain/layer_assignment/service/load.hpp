#pragma once
/// BPP3D domain/layer_assignment/service/load /// 1:1 对应 Rust bpp3d/domain/layer_assignment/service/load.rs
/// Bpp3dDemandEntry + Load

#include <cstddef>
#include <string>
#include <vector>

namespace ospf::framework::bpp3d {

    /// 需求模式 / Demand mode
    /// 对应 Rust Bpp3dDemandMode
    enum class Bpp3dDemandMode {
        Item,
        Cargo
    };

    /// 需求键 / Demand key
    /// 对应 Rust Bpp3dDemandKey
    struct Bpp3dDemandKey {
        Bpp3dDemandMode mode = Bpp3dDemandMode::Item;
        std::string key;

        bool operator==(const Bpp3dDemandKey& other) const noexcept {
            return mode == other.mode && key == other.key;
        }
        bool operator!=(const Bpp3dDemandKey& other) const noexcept { return !(*this == other); }
    };

    /// 需求条目 / Demand entry
    /// 对应 Rust Bpp3dDemandEntry
    struct Bpp3dDemandEntry {
        /// 需求模式 / Demand mode
        Bpp3dDemandMode mode = Bpp3dDemandMode::Item;
        /// 需求键 / Demand key
        Bpp3dDemandKey key;
        /// 需求值 / Demand value
        double demand = 0.0;
    };

    /// 载荷 / Load
    /// 对应 Rust Load
    struct Load {
        /// 总权重 / Total weight
        double total_weight = 0.0;
        /// 总体积 / Total volume
        double total_volume = 0.0;
        /// 总深度 / Total depth
        double total_depth = 0.0;
    };

}  // namespace ospf::framework::bpp3d
