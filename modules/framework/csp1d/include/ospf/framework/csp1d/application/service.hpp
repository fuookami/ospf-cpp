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

        /// 生成器名称 / Generator name
        [[nodiscard]] virtual const std::string& name() const = 0;

        /// 生成定价方案 / Generate pricing plans
        /// 对应 Rust Csp1dPricingGenerator::generate
        [[nodiscard]] virtual std::vector<CuttingPlan> generate() const = 0;

        /// 带上下文生成定价方案 / Generate with context
        /// 对应 Rust Csp1dPricingGenerator::generate_with_context
        [[nodiscard]] virtual std::vector<CuttingPlan> generate_with_context(
            const Csp1dFlowContext& context) const
        {
            return generate();
        }
    };

    // ============================================================================
    // 列生成结果 / Column generation result
    // 对应 Rust Csp1dColumnGenerationResult<V>
    // ============================================================================

    /// 迭代记录 / Iteration record
    struct Csp1dIterationRecord {
        int iteration = 0;
        int plans_before = 0;
        int plans_generated = 0;
        int plans_accepted = 0;
        std::string action;  // "initial_filter" / "pricing" / "warm_start" / "recovery"
    };

    /// 列生成结果 / Column generation result
    struct Csp1dColumnGenerationResult {
        /// 生成的方案列表 / Generated plans
        std::vector<CuttingPlan> generated_plans;

        /// 迭代记录数 / Iteration count
        int iteration_count = 0;

        /// 终止原因 / Termination reason
        std::string termination_reason;

        /// 迭代记录 / Iteration records
        std::vector<Csp1dIterationRecord> iteration_records;

        /// 是否使用了 warm start / Whether warm start was used
        bool used_warm_start = false;

        /// 是否使用了 recovery / Whether recovery was used
        bool used_recovery = false;
    };

    // ============================================================================
    // 列生成服务 / Column generation service
    // 对应 Rust Csp1dColumnGeneration<V>
    // ============================================================================

    /// Warm start 适配器 / Warm start adapter
    class Csp1dWarmStartAdapter {
    public:
        virtual ~Csp1dWarmStartAdapter() = default;

        /// 是否有 warm start 数据 / Has warm start data
        [[nodiscard]] virtual bool has_warm_start() const { return false; }

        /// 获取 warm start 方案 / Get warm start plans
        [[nodiscard]] virtual std::vector<CuttingPlan> get_warm_start_plans() const { return {}; }
    };

    /// Recovery 适配器 / Recovery adapter
    class Csp1dRecoveryAdapter {
    public:
        virtual ~Csp1dRecoveryAdapter() = default;

        /// 尝试恢复 / Attempt recovery
        [[nodiscard]] virtual std::vector<CuttingPlan> recover(
            const Csp1dFlowContext& context) const { return {}; }
    };

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
            std::vector<std::shared_ptr<Csp1dFlowPolicy>> flow_policies = {},
            std::shared_ptr<Csp1dWarmStartAdapter> warm_start = nullptr,
            std::shared_ptr<Csp1dRecoveryAdapter> recovery = nullptr)
            : initial_generator_(std::move(initial_generator))
            , pricing_generator_(std::move(pricing_generator))
            , flow_policies_(std::move(flow_policies))
            , warm_start_(std::move(warm_start))
            , recovery_(std::move(recovery))
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
        /// 1. warm start 检查
        /// 2. 生成初始方案
        /// 3. 流程策略过滤初始方案
        /// 4. 定价迭代（pricing + 过滤 + 合并）
        /// 5. recovery（如果迭代无进展）
        [[nodiscard]] Csp1dColumnGenerationResult solve_with_trace(
            int iteration_limit = 10) const
        {
            Csp1dFlowContext context;
            context.iteration = 0;
            context.iteration_limit = iteration_limit;
            context.allow_partial_solution = true;

            Csp1dColumnGenerationResult result;
            std::vector<Csp1dIterationRecord> records;

            // 1. Warm start 检查 / Warm start check
            if (warm_start_ && warm_start_->has_warm_start()) {
                auto warm_plans = warm_start_->get_warm_start_plans();
                if (!warm_plans.empty()) {
                    Csp1dIterationRecord rec;
                    rec.iteration = 0;
                    rec.plans_before = 0;
                    rec.plans_generated = static_cast<int>(warm_plans.size());
                    rec.plans_accepted = static_cast<int>(warm_plans.size());
                    rec.action = "warm_start";
                    records.push_back(rec);
                    context.current_plans = std::move(warm_plans);
                    result.used_warm_start = true;
                }
            }

            // 2. 生成初始方案 / Generate initial plans
            auto initial_plans = initial_generator_->generate();

            // 3. 流程策略过滤 / Apply flow policy filters
            int plans_before_filter = static_cast<int>(initial_plans.size());
            for (const auto& policy : flow_policies_) {
                initial_plans = policy->filter_initial_plans(context, std::move(initial_plans));
            }

            {
                Csp1dIterationRecord rec;
                rec.iteration = 0;
                rec.plans_before = static_cast<int>(context.current_plans.size());
                rec.plans_generated = plans_before_filter;
                rec.plans_accepted = static_cast<int>(initial_plans.size());
                rec.action = "initial_filter";
                records.push_back(rec);
            }

            for (auto& plan : initial_plans) {
                context.current_plans.push_back(std::move(plan));
            }

            // 4. 定价迭代 / Pricing iteration
            int iteration = 0;
            int consecutive_empty = 0;
            while (iteration < iteration_limit) {
                context.iteration = iteration + 1;
                auto new_plans = pricing_generator_->generate_with_context(context);

                if (new_plans.empty()) {
                    ++consecutive_empty;
                    // 5. Recovery / Recovery attempt
                    if (recovery_ && consecutive_empty >= 2) {
                        auto recovered = recovery_->recover(context);
                        if (!recovered.empty()) {
                            Csp1dIterationRecord rec;
                            rec.iteration = iteration + 1;
                            rec.plans_before = static_cast<int>(context.current_plans.size());
                            rec.plans_generated = static_cast<int>(recovered.size());
                            rec.plans_accepted = static_cast<int>(recovered.size());
                            rec.action = "recovery";
                            records.push_back(rec);
                            for (auto& plan : recovered) {
                                context.current_plans.push_back(std::move(plan));
                            }
                            result.used_recovery = true;
                            consecutive_empty = 0;
                            ++iteration;
                            continue;
                        }
                    }
                    break;
                }

                consecutive_empty = 0;
                int plans_before = static_cast<int>(new_plans.size());

                for (const auto& policy : flow_policies_) {
                    new_plans = policy->filter_initial_plans(context, std::move(new_plans));
                }

                {
                    Csp1dIterationRecord rec;
                    rec.iteration = iteration + 1;
                    rec.plans_before = static_cast<int>(context.current_plans.size());
                    rec.plans_generated = plans_before;
                    rec.plans_accepted = static_cast<int>(new_plans.size());
                    rec.action = "pricing";
                    records.push_back(rec);
                }

                for (auto& plan : new_plans) {
                    context.current_plans.push_back(std::move(plan));
                }
                ++iteration;
            }

            result.generated_plans = context.current_plans;
            result.iteration_count = iteration;
            result.iteration_records = std::move(records);
            result.termination_reason = (iteration >= iteration_limit)
                ? "IterationLimitReached" : "PricingConverged";

            return result;
        }

    private:
        std::shared_ptr<Csp1dInitialCuttingPlanGenerator> initial_generator_;
        std::shared_ptr<Csp1dPricingGenerator> pricing_generator_;
        std::vector<std::shared_ptr<Csp1dFlowPolicy>> flow_policies_;
        std::shared_ptr<Csp1dWarmStartAdapter> warm_start_;
        std::shared_ptr<Csp1dRecoveryAdapter> recovery_;
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
        [[nodiscard]] const std::string& name() const override {
            static const std::string name_ = "EmptyPricingGenerator";
            return name_;
        }

        [[nodiscard]] std::vector<CuttingPlan> generate() const override {
            return {};
        }
    };

}  // namespace ospf::framework::csp1d
