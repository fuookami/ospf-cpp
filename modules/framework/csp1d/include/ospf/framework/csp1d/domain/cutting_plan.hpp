#pragma once
/// CSP1D domain cutting_plan /// 1:1 对应 Rust csp1d/domain/cutting_plan.rs

#include <string>
#include <vector>
#include <optional>

namespace ospf::framework::csp1d {

    /// 切割方案切片 / Cutting plan slice
    /// 对应 Rust CuttingPlanSlice：production + width + amount
    struct CuttingPlanSlice {
        /// 产品 ID / Product ID (对应 Rust CuttingPlanProduction::Product)
        std::string product_id;

        /// 切片宽度 / Slice width
        double width = 0.0;

        /// 切片数量 / Slice amount
        int amount = 1;
    };

    /// 切割方案需求贡献 / Cutting plan demand contribution
    struct CuttingPlanDemandContribution {
        std::string product_id;
        double quantity = 0.0;
    };

    /// 切割方案 / Cutting plan
    /// 对应 Rust CuttingPlan<V>：id + material + slices + demand_contributions + capacity_consumption
    struct CuttingPlan {
        /// 方案 ID / Plan ID
        std::string id;

        /// 材料 ID / Material ID
        std::string material_id;

        /// 机器 ID / Machine ID (optional)
        std::optional<std::string> machine_id;

        /// 切片列表 / Slices
        std::vector<CuttingPlanSlice> slices;

        /// 需求贡献 / Demand contributions
        std::vector<CuttingPlanDemandContribution> demand_contributions;

        /// 容量消耗 / Capacity consumption
        std::optional<double> capacity_consumption;

        /// 方案总宽度 / Total width of the plan
        [[nodiscard]] double total_width() const noexcept {
            double sum = 0.0;
            for (const auto& slice : slices) {
                sum += slice.width * slice.amount;
            }
            return sum;
        }
    };

    /// 切割方案生成报告 / Cutting plan generation report
    /// 对应 Rust CuttingPlanGenerationReport
    struct CuttingPlanGenerationReport {
        std::vector<CuttingPlan> plans;
        /// 生成候选数 / Generated candidate count
        int generated_candidates = 0;
        /// 接受方案数 / Accepted plan count
        int accepted_plans = 0;
    };

}  // namespace ospf::framework::csp1d
