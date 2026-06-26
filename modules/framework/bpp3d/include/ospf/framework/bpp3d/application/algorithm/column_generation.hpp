#pragma once
/// BPP3D application algorithm/column_generation /// 1:1 对应 Rust bpp3d/application/algorithm/column_generation.rs
/// 完整列生成框架：Config / State / Algorithm / ApplicationService
/// 探针已验证控制流；扩展为完整框架含收敛/终止/应用入口

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 领域类型 / Domain types
    // 对应 Rust BinLayer<V, U>
    // ============================================================================

    /// 装箱层 / Packing layer
    /// 对应 Rust BinLayer<V, U>
    struct BinLayer {
        int iteration = 0;
        std::string from;
        double depth = 0.0;
    };

    /// 需求条目 / Demand entry
    struct LayerGenerationDemandEntry {
        std::string item_id;
        int demand = 0;
    };

    /// 层生成请求 / Layer generation request
    /// 对应 Rust LayerGenerationRequest<V, U>
    struct LayerGenerationRequest {
        int iteration = 0;
        std::vector<std::string> item_ids;
        std::vector<BinLayer> existing_layers;
        int max_candidates = 8;
    };

    /// 层生成结果 / Layer generation result
    /// 对应 Rust LayerGenerationResult<V, U>
    struct LayerGenerationResult {
        BinLayer layer;
        double reduced_cost = 0.0;
        double numeric_score = 0.0;
        std::string source;
    };

    // ============================================================================
    // 层生成器接口 / Layer generator interface
    // 对应 Rust LayerGenerator<V, U> trait
    // ============================================================================

    /// 层生成器接口 / Layer generator interface
    /// 对应 Rust LayerGenerator<V, U> trait
    /// 注意：与 application.hpp 的静态 LayerGenerator 不同，这是 trait 式接口
    class IColumnLayerGenerator {
    public:
        virtual ~IColumnLayerGenerator() = default;
        [[nodiscard]] virtual const std::string& name() const = 0;
        [[nodiscard]] virtual std::vector<LayerGenerationResult> generate(
            const LayerGenerationRequest& request) const = 0;
    };

    // ============================================================================
    // 目标方向 / Objective sense
    // 对应 Rust ObjectiveSense
    // ============================================================================

    /// 目标方向 / Objective sense
    /// 对应 Rust ObjectiveSense
    enum class ObjectiveSense {
        Minimize,
        Maximize
    };

    // ============================================================================
    // 列生成配置 / Column generation config
    // 对应 Rust ColumnGenerationConfig（完整字段）
    // ============================================================================

    /// 列生成配置 / Column generation config
    /// 对应 Rust ColumnGenerationConfig
    struct ColumnGenerationConfig {
        /// 最大迭代次数 / Maximum iterations
        int max_iterations = 10;
        /// 最大未改进迭代次数 / Maximum non-improving iterations
        int max_not_better_iterations = 3;
        /// 最大列数量 / Maximum column count
        int max_column_amount = 1000;
        /// 每轮最大候选数 / Maximum candidates per iteration
        int max_candidates_per_iteration = 8;
        /// reduced cost 接受阈值 / Reduced-cost acceptance tolerance
        double reduced_cost_tolerance = 1e-6;
        /// 目标方向 / Objective sense
        ObjectiveSense objective_sense = ObjectiveSense::Minimize;
        /// 迭代限制（兼容探针） / Iteration limit (probe compat)
        int iteration_limit = 10;
    };

    // ============================================================================
    // 列生成状态 / Column generation status
    // 对应 Rust ColumnGenerationStatus（全部变体）
    // ============================================================================

    /// 列生成状态 / Column generation status
    /// 对应 Rust ColumnGenerationStatus（全部变体）
    enum class ColumnGenerationStatus {
        NotStarted,
        Running,
        Converged,
        IterationLimit,
        ColumnLimit,
        TimeLimit,
        Failed
    };

    /// 列生成状态 / Column generation state
    /// 对应 Rust ColumnGenerationState（完整字段）
    struct ColumnGenerationState {
        int iteration = 0;
        int total_columns = 0;
        int not_better_iterations = 0;
        std::optional<double> best_objective;
        ColumnGenerationStatus status = ColumnGenerationStatus::NotStarted;

        /// 是否应该继续 / Should continue
        [[nodiscard]] bool should_continue(const ColumnGenerationConfig& config) const {
            return status == ColumnGenerationStatus::NotStarted
                || (status == ColumnGenerationStatus::Running
                    && iteration < config.iteration_limit
                    && total_columns < config.max_column_amount
                    && not_better_iterations < config.max_not_better_iterations);
        }

        void start() { status = ColumnGenerationStatus::Running; }
        void advance_iteration() { ++iteration; }
        void mark_converged() { status = ColumnGenerationStatus::Converged; }
        void mark_iteration_limit() { status = ColumnGenerationStatus::IterationLimit; }
        void mark_column_limit() { status = ColumnGenerationStatus::ColumnLimit; }
        void mark_time_limit() { status = ColumnGenerationStatus::TimeLimit; }
        void mark_failed() { status = ColumnGenerationStatus::Failed; }

        [[nodiscard]] std::string termination_reason() const {
            switch (status) {
                case ColumnGenerationStatus::Converged: return "PricingConverged";
                case ColumnGenerationStatus::IterationLimit: return "IterationLimitReached";
                case ColumnGenerationStatus::ColumnLimit: return "ColumnLimitReached";
                case ColumnGenerationStatus::TimeLimit: return "TimeLimitReached";
                case ColumnGenerationStatus::Failed: return "Failed";
                case ColumnGenerationStatus::NotStarted: return "NotStarted";
                case ColumnGenerationStatus::Running: return "Running";
            }
            return "Unknown";
        }
    };

    // ============================================================================
    // 列生成结果 / Column generation result
    // 对应 Rust ColumnGenerationResult<V, U>
    // ============================================================================

    /// 列生成结果 / Column generation result
    /// 对应 Rust ColumnGenerationResult<V, U>
    struct ColumnGenerationResult {
        ColumnGenerationState state;
        std::vector<BinLayer> layers;
        std::unordered_map<std::string, std::string> info;

        [[nodiscard]] const std::string& termination_reason() const {
            return info.at("termination_reason");
        }
    };

    // ============================================================================
    // 列生成算法 / Column generation algorithm
    // 对应 Rust ColumnGenerationAlgorithm<V, U>
    // 完整框架：generate_once / add_columns / remove_columns / active_layers / should_continue / result
    // ============================================================================

    /// 列生成算法 / Column generation algorithm
    /// 对应 Rust ColumnGenerationAlgorithm<V, U>
    class ColumnGenerationAlgorithm {
    public:
        explicit ColumnGenerationAlgorithm(ColumnGenerationConfig config)
            : config_(std::move(config)) {}

        /// 添加初始层 / Add initial layers
        std::vector<BinLayer> add_initial_layers(std::vector<BinLayer> layers) {
            for (auto& layer : layers) {
                layer.iteration = 0;
                layer_aggregation_.push_back(layer);
                ++state_.total_columns;
            }
            return layers;
        }

        /// 添加列 / Add columns
        void add_columns(int iteration, std::vector<BinLayer> layers) {
            for (auto& layer : layers) {
                layer.iteration = iteration;
                layer_aggregation_.push_back(layer);
                ++state_.total_columns;
            }
        }

        /// 移除列 / Remove columns
        void remove_columns(const std::vector<std::size_t>& column_indices) {
            for (std::size_t idx : column_indices) {
                if (idx < layer_aggregation_.size() && !removed_.count(idx)) {
                    removed_.insert(idx);
                    ++removed_count_;
                }
            }
        }

        /// 活跃层 / Active layers
        [[nodiscard]] std::vector<BinLayer> active_layers() const {
            std::vector<BinLayer> result;
            for (std::size_t i = 0; i < layer_aggregation_.size(); ++i) {
                if (!removed_.count(i)) {
                    result.push_back(layer_aggregation_[i]);
                }
            }
            return result;
        }

        [[nodiscard]] std::size_t active_column_count() const {
            return layer_aggregation_.size() - removed_count_;
        }

        [[nodiscard]] std::size_t removed_column_count() const {
            return removed_count_;
        }

        /// 生成一次 / Generate once
        /// 核心控制流：构造请求 → 生成层 → 添加列 → 收敛检查 → 推进迭代
        [[nodiscard]] std::vector<LayerGenerationResult> generate_once(
            const std::vector<std::string>& item_ids,
            const std::vector<LayerGenerationDemandEntry>& /*demand_entries*/)
        {
            if (state_.status == ColumnGenerationStatus::NotStarted) {
                state_.start();
            }

            LayerGenerationRequest request;
            request.iteration = state_.iteration;
            request.item_ids = item_ids;
            request.existing_layers = active_layers();
            request.max_candidates = config_.max_candidates_per_iteration;

            std::vector<LayerGenerationResult> results;
            for (const auto& generator : generators_) {
                auto generated = generator->generate(request);
                for (auto& r : generated) {
                    results.push_back(std::move(r));
                }
            }

            // 检查 reduced cost 改善 / Check reduced cost improvement
            bool improved = false;
            for (const auto& r : results) {
                if (r.reduced_cost < -config_.reduced_cost_tolerance) {
                    improved = true;
                    break;
                }
            }

            // 提取层并添加为列 / Extract layers and add as columns
            std::vector<BinLayer> new_layers;
            for (const auto& result : results) {
                new_layers.push_back(result.layer);
            }
            add_columns(state_.iteration, std::move(new_layers));

            // 更新未改进计数 / Update non-improving count
            if (improved) {
                state_.not_better_iterations = 0;
            } else {
                ++state_.not_better_iterations;
            }

            state_.advance_iteration();

            // 终止判断 / Termination check
            if (state_.not_better_iterations >= config_.max_not_better_iterations) {
                state_.mark_converged();
            } else if (state_.iteration >= config_.iteration_limit) {
                state_.mark_iteration_limit();
            } else if (state_.total_columns >= config_.max_column_amount) {
                state_.mark_column_limit();
            }

            return results;
        }

        [[nodiscard]] bool should_continue() const {
            return state_.should_continue(config_);
        }

        [[nodiscard]] ColumnGenerationResult result() const {
            ColumnGenerationResult r;
            r.state = state_;
            r.layers = active_layers();
            r.info["framework_lifecycle_active_column_count"] = std::to_string(active_column_count());
            r.info["framework_lifecycle_removed_column_count"] = std::to_string(removed_count_);
            r.info["termination_reason"] = state_.termination_reason();
            return r;
        }

        void add_generator(std::shared_ptr<IColumnLayerGenerator> generator) {
            generators_.push_back(std::move(generator));
        }

        [[nodiscard]] const ColumnGenerationState& state() const { return state_; }
        [[nodiscard]] const ColumnGenerationConfig& config() const { return config_; }

    private:
        ColumnGenerationConfig config_;
        ColumnGenerationState state_;
        std::vector<BinLayer> layer_aggregation_;
        std::unordered_set<std::size_t> removed_;
        std::size_t removed_count_ = 0;
        std::vector<std::shared_ptr<IColumnLayerGenerator>> generators_;
    };

    // ============================================================================
    // 列生成应用服务 / Column generation application service
    // 对应 Rust ColumnGenerationApplicationService
    // ============================================================================

    /// 列生成应用服务 / Column generation application service
    /// 对应 Rust ColumnGenerationApplicationService
    class ColumnGenerationApplicationService {
    public:
        explicit ColumnGenerationApplicationService(ColumnGenerationConfig config = {})
            : config_(std::move(config)) {}

        [[nodiscard]] ColumnGenerationApplicationService create_algorithm() const {
            return ColumnGenerationApplicationService(config_);
        }

        [[nodiscard]] const ColumnGenerationConfig& config() const {
            return config_;
        }

        /// 运行列生成 / Run column generation
        /// 端到端入口：创建算法 → 添加生成器 → 迭代求解 → 返回结果
        [[nodiscard]] ColumnGenerationResult run(
            std::vector<std::shared_ptr<IColumnLayerGenerator>> generators,
            std::vector<BinLayer> initial_layers = {},
            std::vector<std::string> item_ids = {}) const
        {
            ColumnGenerationAlgorithm algorithm(config_);
            for (auto& gen : generators) {
                algorithm.add_generator(gen);
            }
            if (!initial_layers.empty()) {
                algorithm.add_initial_layers(std::move(initial_layers));
            }

            while (algorithm.should_continue()) {
                auto results = algorithm.generate_once(item_ids, {});
                if (results.empty()) {
                    break;
                }
            }

            return algorithm.result();
        }

    private:
        ColumnGenerationConfig config_;
    };

    // ============================================================================
    // MetaModel 执行诊断 / MetaModel execution diagnostics
    // 对应 Rust MetaModelExecutionDiagnostics
    // ============================================================================

    /// MetaModel 执行诊断 / MetaModel execution diagnostics
    /// 对应 Rust MetaModelExecutionDiagnostics
    struct MetaModelExecutionDiagnostics {
        std::string model_name;
        std::size_t variable_count = 0;
        std::size_t constraint_count = 0;
        std::size_t demand_count = 0;
        std::size_t layer_count = 0;
        std::size_t bin_count = 0;
    };

    // ============================================================================
    // MetaModel 求解结果 / MetaModel executor solve result
    // 对应 Rust MetaModelExecutorSolveResult
    // ============================================================================

    /// MetaModel 求解结果 / MetaModel executor solve result
    /// 对应 Rust MetaModelExecutorSolveResult
    struct MetaModelExecutorSolveResult {
        /// 目标值 / Objective value
        std::optional<double> objective;
        /// 变量值 / Variable values
        std::unordered_map<std::string, double> variable_values;
        /// 影子价格 / Shadow prices
        std::unordered_map<std::string, double> shadow_prices;
        /// 信息 / Info
        std::unordered_map<std::string, std::string> info;
    };

    // ============================================================================
    // RMP 执行结果 / RMP execution result
    // 对应 Rust ColumnGenerationRmpExecution
    // ============================================================================

    /// RMP 执行结果 / RMP execution result
    /// 对应 Rust ColumnGenerationRmpExecution
    struct ColumnGenerationRmpExecution {
        /// 目标值 / Objective
        std::optional<double> objective;
        /// 影子价格汇总 / Shadow price summary
        std::unordered_map<std::string, double> shadow_price_summary;
        /// 诊断 / Diagnostics
        std::optional<MetaModelExecutionDiagnostics> diagnostics;
        /// 信息 / Info
        std::unordered_map<std::string, std::string> info;
    };

    // ============================================================================
    // Final MILP 执行结果 / Final MILP execution result
    // 对应 Rust ColumnGenerationFinalExecution
    // ============================================================================

    /// Final MILP 执行结果 / Final MILP execution result
    /// 对应 Rust ColumnGenerationFinalExecution
    struct ColumnGenerationFinalExecution {
        /// 活跃层 / Layers
        std::vector<BinLayer> layers;
        /// 目标值 / Objective
        std::optional<double> objective;
        /// 诊断 / Diagnostics
        std::optional<MetaModelExecutionDiagnostics> diagnostics;
        /// 信息 / Info
        std::unordered_map<std::string, std::string> info;
    };

    // ============================================================================
    // MetaModel 求解器后端接口 / MetaModel solver backend interface
    // 对应 Rust MetaModelSolverBackend trait
    // ============================================================================

    /// MetaModel 求解器后端接口 / MetaModel solver backend interface
    /// 对应 Rust MetaModelSolverBackend trait
    class MetaModelSolverBackend {
    public:
        virtual ~MetaModelSolverBackend() = default;

        /// 后端名称 / Backend name
        [[nodiscard]] virtual const std::string& name() const = 0;

        /// 求解 RMP LP / Solve RMP LP
        /// 对应 Rust MetaModelSolverBackend::solve_rmp
        [[nodiscard]] virtual MetaModelExecutorSolveResult solve_rmp(
            const MetaModelExecutionDiagnostics& diagnostics) = 0;

        /// 求解 final MILP / Solve final MILP
        /// 对应 Rust MetaModelSolverBackend::solve_final
        [[nodiscard]] virtual MetaModelExecutorSolveResult solve_final(
            const MetaModelExecutionDiagnostics& diagnostics) = 0;
    };

    // ============================================================================
    // Noop MetaModel 求解器后端 / Noop MetaModel solver backend
    // 对应 Rust NoopMetaModelSolverBackend
    // ============================================================================

    /// Noop MetaModel 求解器后端 / Noop MetaModel solver backend
    /// 对应 Rust NoopMetaModelSolverBackend（返回默认结果，用于测试）
    class NoopMetaModelSolverBackend : public MetaModelSolverBackend {
    public:
        [[nodiscard]] const std::string& name() const override {
            static const std::string n = "noop";
            return n;
        }

        [[nodiscard]] MetaModelExecutorSolveResult solve_rmp(
            const MetaModelExecutionDiagnostics&) override {
            return MetaModelExecutorSolveResult{};
        }

        [[nodiscard]] MetaModelExecutorSolveResult solve_final(
            const MetaModelExecutionDiagnostics&) override {
            return MetaModelExecutorSolveResult{};
        }
    };

    // ============================================================================
    // RMP 执行器接口 / RMP executor interface
    // 对应 Rust ColumnGenerationRmpExecutor trait
    // ============================================================================

    /// RMP 执行器接口 / RMP executor interface
    /// 对应 Rust ColumnGenerationRmpExecutor trait
    class ColumnGenerationRmpExecutor {
    public:
        virtual ~ColumnGenerationRmpExecutor() = default;

        /// 执行 RMP / Execute RMP
        /// 对应 Rust ColumnGenerationRmpExecutor::execute
        [[nodiscard]] virtual ColumnGenerationRmpExecution execute(
            const std::vector<BinLayer>& layers,
            const std::vector<std::string>& item_ids) = 0;
    };

    // ============================================================================
    // Final MILP 执行器接口 / Final MILP executor interface
    // 对应 Rust ColumnGenerationFinalExecutor trait
    // ============================================================================

    /// Final MILP 执行器接口 / Final MILP executor interface
    /// 对应 Rust ColumnGenerationFinalExecutor trait
    class ColumnGenerationFinalExecutor {
    public:
        virtual ~ColumnGenerationFinalExecutor() = default;

        /// 执行 final MILP / Execute final MILP
        /// 对应 Rust ColumnGenerationFinalExecutor::execute
        [[nodiscard]] virtual ColumnGenerationFinalExecution execute(
            const std::vector<BinLayer>& layers,
            const std::vector<std::string>& item_ids) = 0;
    };

}  // namespace ospf::framework::bpp3d
