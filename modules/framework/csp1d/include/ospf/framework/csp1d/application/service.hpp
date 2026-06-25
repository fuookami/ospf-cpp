#pragma once
/// CSP1D application/service /// 1:1 对应 Rust csp1d/application/service.rs
/// 列生成框架：FlowPolicy + InitialCuttingPlanGenerator + PricingGenerator + Csp1dColumnGeneration

#include <ospf/framework/csp1d/domain/cutting_plan.hpp>

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace ospf::framework::csp1d {

    // ============================================================================
    // 流程上下文接口 / Flow context interface
    // 对应 Rust Csp1dFlowContext
    // ============================================================================

    /// 流程上下文 / Flow context
    /// 对应 Rust Csp1dFlowContext<V> trait
    struct Csp1dFlowContext {
        int iteration = 0;
        std::vector<CuttingPlan> current_plans;
        int iteration_limit = 10;
        bool allow_partial_solution = true;
    };

    // ============================================================================
    // 流程策略接口 / Flow policy interface
    // 对应 Rust Csp1dFlowPolicy<V> trait
    // ============================================================================

    /// 流程策略 / Flow policy
    /// 对应 Rust Csp1dFlowPolicy<V> trait
    /// 控制列生成迭代中的方案过滤、等价判断、部分方案接受
    class Csp1dFlowPolicy {
    public:
        virtual ~Csp1dFlowPolicy() = default;

        /// 策略名称 / Policy name
        [[nodiscard]] virtual const std::string& name() const = 0;

        /// 过滤初始方案 / Filter initial plans
        /// 对应 Rust Csp1dFlowPolicy::filter_initial_plans
        [[nodiscard]] virtual std::vector<CuttingPlan> filter_initial_plans(
            const Csp1dFlowContext& context,
            std::vector<CuttingPlan> plans) const = 0;

        /// 判断两个方案是否等价 / Check equivalence of two plans
        /// 对应 Rust Csp1dFlowPolicy::is_equivalent
        [[nodiscard]] virtual bool is_equivalent(
            const Csp1dFlowContext& context,
            const CuttingPlan& existing,
            const CuttingPlan& candidate) const = 0;

        /// 是否接受部分方案 / Accept partial solution
        /// 对应 Rust Csp1dFlowPolicy::accept_partial
        [[nodiscard]] virtual bool accept_partial(
            const Csp1dFlowContext& context) const = 0;
    };

    // ============================================================================
    // 初始切割方案生成器接口 / Initial cutting plan generator
    // 对应 Rust Csp1dInitialCuttingPlanGenerator<V> trait
    // ============================================================================

    /// 初始切割方案生成器 / Initial cutting plan generator
    /// 对应 Rust Csp1dInitialCuttingPlanGenerator<V> trait
    class Csp1dInitialCuttingPlanGenerator {
    public:
        virtual ~Csp1dInitialCuttingPlanGenerator() = default;

        /// 生成初始方案 / Generate initial plans
        /// 对应 Rust Csp1dInitialCuttingPlanGenerator::generate
        [[nodiscard]] virtual std::vector<CuttingPlan> generate() const = 0;

        /// 生成初始方案（带报告） / Generate with report
        /// 对应 Rust Csp1dInitialCuttingPlanGenerator::generate_with_report
        [[nodiscard]] virtual CuttingPlanGenerationReport generate_with_report() const = 0;
    };

    // ============================================================================
    // 定价生成器接口 / Pricing generator
    // 对应 Rust Csp1dPricingGenerator<V> trait
    // ============================================================================

    /// 定价生成器 / Pricing generator
    /// 对应 Rust Csp1dPricingGenerator<V> trait
    class Csp1dPricingGenerator {
    public:
        virtual ~Csp1dPricingGenerator() = default;

        /// 生成定价方案 / Generate pricing plans
        /// 对应 Rust Csp1dPricingGenerator::generate
        [[nodiscard]] virtual std::vector<CuttingPlan> generate() const = 0;
    };

    // ============================================================================
    // 列生成结果 / Column generation result
    // 对应 Rust Csp1dColumnGenerationResult<V>
    // ============================================================================

    /// 列生成结果 / Column generation result
    struct Csp1dColumnGenerationResult {
        /// 生成的方案列表 / Generated plans
        std::vector<CuttingPlan> generated_plans;

        /// 迭代记录数 / Iteration count
        int iteration_count = 0;

        /// 终止原因 / Termination reason
        std::string termination_reason;
    };

    // ============================================================================
    // 列生成服务 / Column generation service
    // 对应 Rust Csp1dColumnGeneration<V>
    // ============================================================================

    /// 列生成服务 / Column generation service
    /// 对应 Rust Csp1dColumnGeneration<V>
    /// 实现列生成控制流：初始方案生成 → flow policy 过滤 → pricing 迭代
    class Csp1dColumnGeneration {
    public:
        /// 使用指定生成器构造 / Construct with specified generators
        /// 对应 Rust Csp1dColumnGeneration::with_generators
        Csp1dColumnGeneration(
            std::shared_ptr<Csp1dInitialCuttingPlanGenerator> initial_generator,
            std::shared_ptr<Csp1dPricingGenerator> pricing_generator,
            std::vector<std::shared_ptr<Csp1dFlowPolicy>> flow_policies = {})
            : initial_generator_(std::move(initial_generator))
            , pricing_generator_(std::move(pricing_generator))
            , flow_policies_(std::move(flow_policies))
        {}

        /// 求解（返回方案列表） / Solve (return plans)
        /// 对应 Rust Csp1dColumnGeneration::solve
        [[nodiscard]] Csp1dColumnGenerationResult solve(
            int iteration_limit = 10) const
        {
            return solve_with_trace(iteration_limit);
        }

        /// 求解（带追踪） / Solve with trace
        /// 对应 Rust Csp1dColumnGeneration::solve_with_trace
        /// 核心列生成控制流：
        /// 1. 生成初始方案
        /// 2. 流程策略过滤初始方案
        /// 3. 定价迭代（预留，probe 只验证 1-2）
        [[nodiscard]] Csp1dColumnGenerationResult solve_with_trace(
            int iteration_limit = 10) const
        {
            Csp1dFlowContext context;
            context.iteration = 0;
            context.iteration_limit = iteration_limit;
            context.allow_partial_solution = true;

            // 1. 生成初始方案 / Generate initial plans
            auto initial_plans = initial_generator_->generate();

            // 2. 流程策略过滤初始方案 / Apply flow policy filters
            for (const auto& policy : flow_policies_) {
                initial_plans = policy->filter_initial_plans(context, std::move(initial_plans));
            }

            context.current_plans = initial_plans;

            // 3. 定价迭代（probe 阶段：pricing generator 返回空，直接结束）
            // Pricing iteration (probe: pricing returns empty, terminate)
            int iteration = 0;
            while (iteration < iteration_limit) {
                auto new_plans = pricing_generator_->generate();
                if (new_plans.empty()) {
                    break;  // pricing 无新方案，收敛 / No new plans, converged
                }

                // 过滤新方案 / Filter new plans through flow policies
                for (const auto& policy : flow_policies_) {
                    new_plans = policy->filter_initial_plans(context, std::move(new_plans));
                }

                // 合并 / Merge
                for (auto& plan : new_plans) {
                    context.current_plans.push_back(std::move(plan));
                }
                ++iteration;
            }

            Csp1dColumnGenerationResult result;
            result.generated_plans = context.current_plans;
            result.iteration_count = iteration;
            result.termination_reason = (iteration >= iteration_limit)
                ? "IterationLimitReached" : "PricingConverged";

            return result;
        }

    private:
        std::shared_ptr<Csp1dInitialCuttingPlanGenerator> initial_generator_;
        std::shared_ptr<Csp1dPricingGenerator> pricing_generator_;
        std::vector<std::shared_ptr<Csp1dFlowPolicy>> flow_policies_;
    };

    // ============================================================================
    // 便捷生成器实现 / Convenience generator implementations
    // 用于测试和探针 / For tests and probes
    // ============================================================================

    /// 固定方案枚举器 / Fixed plan enumerator
    /// 对应 Rust 测试中的 FixedPlanEnumerator
    class FixedPlanEnumerator : public Csp1dInitialCuttingPlanGenerator {
    public:
        explicit FixedPlanEnumerator(std::vector<CuttingPlan> plans)
            : plans_(std::move(plans)) {}

        [[nodiscard]] std::vector<CuttingPlan> generate() const override {
            return plans_;
        }

        [[nodiscard]] CuttingPlanGenerationReport generate_with_report() const override {
            CuttingPlanGenerationReport report;
            report.plans = plans_;
            report.generated_candidates = static_cast<int>(plans_.size());
            report.accepted_plans = static_cast<int>(plans_.size());
            return report;
        }

    private:
        std::vector<CuttingPlan> plans_;
    };

    /// 空定价生成器 / Empty pricing generator
    /// 对应 Rust 测试中的 EmptyPricingGenerator
    class EmptyPricingGenerator : public Csp1dPricingGenerator {
    public:
        [[nodiscard]] std::vector<CuttingPlan> generate() const override {
            return {};
        }
    };

}  // namespace ospf::framework::csp1d
