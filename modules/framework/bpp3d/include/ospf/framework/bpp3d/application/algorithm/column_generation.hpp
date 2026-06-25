#pragma once
/// BPP3D application algorithm/column_generation /// 1:1 对应 Rust bpp3d/application/algorithm/column_generation.rs
/// 列生成框架：ColumnGenerationAlgorithm 控制流（generate_once / add_columns / remove_columns / active_layers）
/// 探针：验证 C++ 列生成控制流与 Rust ColumnGenerationAlgorithm 行为一致

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 领域类型（探针最小版）/ Domain types (probe minimal)
    // 对应 Rust BinLayer<V, U>
    // ============================================================================

    /// 装箱层 / Packing layer
    /// 对应 Rust BinLayer<V, U>
    struct BinLayer {
        /// 迭代号 / Iteration
        int iteration = 0;

        /// 来源（生成器名） / Source (generator name)
        std::string from;

        /// 层深度 / Layer depth
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

        /// 生成器名称 / Generator name
        [[nodiscard]] virtual const std::string& name() const = 0;

        /// 生成层 / Generate layers
        /// 对应 Rust LayerGenerator::generate
        [[nodiscard]] virtual std::vector<LayerGenerationResult> generate(
            const LayerGenerationRequest& request) const = 0;
    };

    // ============================================================================
    // 列生成状态 / Column generation state
    // 对应 Rust ColumnGenerationStatus + ColumnGenerationApplicationState
    // ============================================================================

    /// 列生成状态 / Column generation status
    enum class ColumnGenerationStatus {
        NotStarted,
        Running,
        Finished
    };

    /// 列生成应用状态 / Column generation application state
    /// 对应 Rust ColumnGenerationApplicationState
    struct ColumnGenerationApplicationState {
        ColumnGenerationStatus status = ColumnGenerationStatus::NotStarted;
        int iteration = 0;
        int total_columns = 0;
        std::vector<BinLayer> layers;

        /// 启动 / Start
        void start() {
            status = ColumnGenerationStatus::Running;
        }

        /// 推进迭代 / Advance iteration
        void advance_iteration() {
            ++iteration;
        }
    };

    // ============================================================================
    // 列生成配置 / Column generation config
    // 对应 Rust ColumnGenerationConfig
    // ============================================================================

    /// 列生成配置 / Column generation config
    struct ColumnGenerationConfig {
        int max_candidates_per_iteration = 8;
        int iteration_limit = 10;
    };

    // ============================================================================
    // 列生成结果 / Column generation result
    // 对应 Rust ColumnGenerationResult<V, U>
    // ============================================================================

    /// 列生成结果 / Column generation result
    struct ColumnGenerationResult {
        ColumnGenerationApplicationState state;
        std::vector<BinLayer> layers;
        std::unordered_map<std::string, std::string> info;
    };

    // ============================================================================
    // 列生成算法 / Column generation algorithm
    // 对应 Rust ColumnGenerationAlgorithm<V, U>
    // 核心控制流：generate_once → layer generation → add_columns → advance_iteration
    // ============================================================================

    /// 列生成算法 / Column generation algorithm
    /// 对应 Rust ColumnGenerationAlgorithm<V, U>
    class ColumnGenerationAlgorithm {
    public:
        /// 构造 / Construct
        explicit ColumnGenerationAlgorithm(ColumnGenerationConfig config)
            : config_(std::move(config)) {}

        /// 添加初始层 / Add initial layers
        /// 对应 Rust ColumnGenerationAlgorithm::add_initial_layers
        std::vector<BinLayer> add_initial_layers(std::vector<BinLayer> layers) {
            for (auto& layer : layers) {
                layer.iteration = 0;
                layer_aggregation_.push_back(layer);
                ++state_.total_columns;
            }
            return layers;
        }

        /// 添加列 / Add columns
        /// 对应 Rust ColumnGenerationAlgorithm::add_columns
        void add_columns(int iteration, std::vector<BinLayer> layers) {
            for (auto& layer : layers) {
                layer.iteration = iteration;
                layer_aggregation_.push_back(layer);
                ++state_.total_columns;
            }
        }

        /// 移除列 / Remove columns
        /// 对应 Rust ColumnGenerationAlgorithm::remove_columns
        void remove_columns(const std::vector<std::size_t>& column_indices) {
            for (std::size_t idx : column_indices) {
                if (idx < layer_aggregation_.size() && !removed_.count(idx)) {
                    removed_.insert(idx);
                    ++removed_count_;
                }
            }
        }

        /// 活跃层 / Active layers
        /// 对应 Rust ColumnGenerationAlgorithm::active_layers
        [[nodiscard]] std::vector<BinLayer> active_layers() const {
            std::vector<BinLayer> result;
            for (std::size_t i = 0; i < layer_aggregation_.size(); ++i) {
                if (!removed_.count(i)) {
                    result.push_back(layer_aggregation_[i]);
                }
            }
            return result;
        }

        /// 活跃列数 / Active column count
        [[nodiscard]] std::size_t active_column_count() const {
            return layer_aggregation_.size() - removed_count_;
        }

        /// 已移除列数 / Removed column count
        [[nodiscard]] std::size_t removed_column_count() const {
            return removed_count_;
        }

        /// 生成一次 / Generate once
        /// 对应 Rust ColumnGenerationAlgorithm::generate_once
        /// 核心控制流：构造请求 → 生成层 → 添加列 → 推进迭代
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

            // 提取层并添加为列 / Extract layers and add as columns
            std::vector<BinLayer> new_layers;
            for (const auto& result : results) {
                new_layers.push_back(result.layer);
            }
            add_columns(state_.iteration, std::move(new_layers));

            state_.advance_iteration();
            return results;
        }

        /// 是否继续 / Should continue
        /// 对应 Rust ColumnGenerationAlgorithm::should_continue
        [[nodiscard]] bool should_continue() const {
            return state_.iteration < config_.iteration_limit;
        }

        /// 构造结果 / Build result
        /// 对应 Rust ColumnGenerationAlgorithm::result
        [[nodiscard]] ColumnGenerationResult result() const {
            ColumnGenerationResult r;
            r.state = state_;
            r.layers = active_layers();
            r.info["framework_lifecycle_active_column_count"] = std::to_string(active_column_count());
            r.info["framework_lifecycle_removed_column_count"] = std::to_string(removed_count_);
            return r;
        }

        /// 添加生成器 / Add generator
        void add_generator(std::shared_ptr<IColumnLayerGenerator> generator) {
            generators_.push_back(std::move(generator));
        }

    private:
        ColumnGenerationConfig config_;
        ColumnGenerationApplicationState state_;
        std::vector<BinLayer> layer_aggregation_;
        std::unordered_set<std::size_t> removed_;
        std::size_t removed_count_ = 0;
        std::vector<std::shared_ptr<IColumnLayerGenerator>> generators_;
    };

}  // namespace ospf::framework::bpp3d
