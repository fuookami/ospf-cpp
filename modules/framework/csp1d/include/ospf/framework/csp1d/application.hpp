#pragma once

/// CSP1D 应用层 / CSP1D application layer
/// 切割算法和模式生成。

#include <ospf/framework/csp1d/domain.hpp>
#include <algorithm>
#include <limits>
#include <vector>

namespace ospf::framework::csp1d {

    /// 切割算法基类 / Cutting algorithm base class
    class CuttingAlgorithm {
    public:
        virtual ~CuttingAlgorithm() = default;

        /// 生成切割方案 / Generate cutting patterns
        [[nodiscard]] virtual ProductionPlan generate(
            const std::vector<Stock>& stocks,
            const std::vector<Piece>& pieces) = 0;

        /// 算法名称 / Algorithm name
        [[nodiscard]] virtual const std::string& name() const = 0;
    };

    /// 首次适应算法 / First Fit algorithm
    class FirstFitCutting : public CuttingAlgorithm {
    public:
        [[nodiscard]] ProductionPlan generate(
            const std::vector<Stock>& stocks,
            const std::vector<Piece>& pieces) override
        {
            ProductionPlan plan;

            // 为每种需求件创建需求列表
            std::vector<std::size_t> remaining(pieces.size());
            for (std::size_t i = 0; i < pieces.size(); ++i) {
                remaining[i] = pieces[i].demand;
            }

            // 对每根原料尝试切割
            for (const auto& stock : stocks) {
                for (std::size_t q = 0; q < stock.quantity; ++q) {
                    double remaining_length = stock.length;
                    std::vector<std::size_t> pattern_counts(pieces.size(), 0);

                    // 贪心：尽可能多地放入需求件
                    for (std::size_t i = 0; i < pieces.size(); ++i) {
                        while (remaining[i] > 0 && remaining_length >= pieces[i].length) {
                            pattern_counts[i]++;
                            remaining[i]--;
                            remaining_length -= pieces[i].length;
                        }
                    }

                    // 检查是否有切割
                    bool has_cuts = false;
                    for (auto count : pattern_counts) {
                        if (count > 0) {
                            has_cuts = true;
                            break;
                        }
                    }

                    if (has_cuts) {
                        CuttingPattern pattern;
                        pattern.stock_id = stock.id;
                        pattern.piece_counts = pattern_counts;
                        pattern.waste = remaining_length;
                        plan.patterns.push_back(pattern);
                        plan.total_waste += remaining_length;
                    }
                }
            }

            return plan;
        }

        [[nodiscard]] const std::string& name() const override {
            static const std::string n = "FirstFitCutting";
            return n;
        }
    };

    /// 计算产出 / Calculate yield
    [[nodiscard]] inline YieldResult calculate_yield(
        const ProductionPlan& plan,
        const std::vector<Piece>& pieces)
    {
        YieldResult result;
        result.demanded = 0;
        for (const auto& piece : pieces) {
            result.demanded += piece.demand;
        }

        result.produced = plan.total_pieces(pieces);
        result.waste_length = plan.total_waste;

        return result;
    }

    /// 计算余料 / Calculate waste
    [[nodiscard]] inline double calculate_waste(
        const ProductionPlan& plan,
        const std::vector<Stock>& stocks)
    {
        double total_stock_length = 0.0;
        for (const auto& stock : stocks) {
            total_stock_length += stock.total_length();
        }
        return plan.total_waste;
    }

}  // namespace ospf::framework::csp1d
