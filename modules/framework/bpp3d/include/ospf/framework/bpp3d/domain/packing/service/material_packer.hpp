#pragma once
/// BPP3D domain/packing/service/material_packer /// 1:1 对应 Rust bpp3d/domain/packing/service/material_packer.rs
/// MaterialPacker + MaterialSummary extraction

#include <ospf/framework/bpp3d/domain/packing/model.hpp>
#include <ospf/framework/bpp3d/domain/item/model/item.hpp>

#include <algorithm>
#include <string>
#include <vector>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 物料包装器 / Material packer
    // 对应 Rust MaterialPacker
    // ============================================================================

    /// 物料包装器 / Material packer
    /// 对应 Rust MaterialPacker
    struct MaterialPacker {
        /// 创建 / Create
        [[nodiscard]] static MaterialPacker create() {
            return MaterialPacker{};
        }

        /// 物料汇总 / Invoke material summary
        /// 对应 Rust MaterialPacker::invoke
        /// 从 PackingResult 的 PackedBin.items 中提取物料汇总
        [[nodiscard]] std::vector<MaterialSummary> invoke(const PackingResult& result) const {
            std::vector<MaterialSummary> summaries;
            for (const auto& bin : result.packed_bins) {
                for (const auto& item : bin.items) {
                    // 每个 item 贡献 1 个物料（简化实现）
                    MaterialKey key{"DEFAULT", MaterialType::RawMaterial};
                    bool found = false;
                    for (auto& summary : summaries) {
                        if (summary.material == key) {
                            summary.amount += 1;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        summaries.push_back({key, 1});
                    }
                }
            }
            return summaries;
        }

        /// 物料汇总（从 PackingResult 的 material_summaries 直接读取）
        /// 对应 Rust MaterialPacker::invoke (from result.material_summaries)
        [[nodiscard]] std::vector<MaterialSummary> invoke_from_result(const PackingResult& result) const {
            return result.material_summaries;
        }
    };

}  // namespace ospf::framework::bpp3d
