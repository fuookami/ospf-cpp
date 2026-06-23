#pragma once

/// CSP1D 领域模型 / CSP1D domain model
/// 一维下料问题的领域类型：Material、Stock、Piece。

#include <cstdint>
#include <string>
#include <vector>
#include <optional>

namespace ospf::framework::csp1d {

    /// 材料 / Material
    struct Material {
        std::string id;
        std::string name;
        double unit_cost = 0.0;
    };

    /// 原料（库存） / Stock (raw material)
    struct Stock {
        std::string id;
        Material material;
        double length = 0.0;
        std::size_t quantity = 0;

        [[nodiscard]] double total_length() const noexcept {
            return length * static_cast<double>(quantity);
        }
    };

    /// 需求件（切割目标） / Piece (cut target)
    struct Piece {
        std::string id;
        Material material;
        double length = 0.0;
        std::size_t demand = 0;

        [[nodiscard]] double total_length() const noexcept {
            return length * static_cast<double>(demand);
        }
    };

    /// 切割方案 / Cutting pattern
    struct CuttingPattern {
        std::string id;
        std::string stock_id;
        std::vector<std::size_t> piece_counts;  // 每种需求件的数量
        double waste = 0.0;

        [[nodiscard]] double used_length(const std::vector<Piece>& pieces) const {
            double used = 0.0;
            for (std::size_t i = 0; i < piece_counts.size() && i < pieces.size(); ++i) {
                used += pieces[i].length * static_cast<double>(piece_counts[i]);
            }
            return used;
        }
    };

    /// 生产计划 / Production plan
    struct ProductionPlan {
        std::vector<CuttingPattern> patterns;
        double total_waste = 0.0;
        double total_cost = 0.0;

        [[nodiscard]] std::size_t total_pieces(const std::vector<Piece>& pieces) const {
            std::size_t total = 0;
            for (const auto& pattern : patterns) {
                for (std::size_t i = 0; i < pattern.piece_counts.size() && i < pieces.size(); ++i) {
                    total += pattern.piece_counts[i];
                }
            }
            return total;
        }
    };

    /// 产出结果 / Yield result
    struct YieldResult {
        std::size_t produced = 0;
        std::size_t demanded = 0;
        double waste_length = 0.0;

        [[nodiscard]] double yield_rate() const noexcept {
            if (demanded == 0) return 0.0;
            return static_cast<double>(produced) / static_cast<double>(demanded);
        }
    };

}  // namespace ospf::framework::csp1d
