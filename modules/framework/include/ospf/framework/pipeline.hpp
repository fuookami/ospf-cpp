#pragma once

/// 约束管线 / Constraint pipeline
/// 管理约束的注册、验证、转换。

#include <ospf/core/model.hpp>
#include <functional>
#include <vector>

namespace ospf::framework {

    /// 约束管线阶段 / Constraint pipeline stage
    enum class PipelineStage : std::uint8_t {
        Registration,  ///< 注册阶段
        Validation,    ///< 验证阶段
        Transformation, ///< 转换阶段
        Finalization,  ///< 完成阶段
    };

    /// 约束管线 / Constraint pipeline
    class ConstraintPipeline {
    public:
        using StageFunc = std::function<void(ospf::core::MetaModel&, PipelineStage)>;

        /// 添加管线阶段 / Add pipeline stage
        void add_stage(StageFunc func) {
            stages_.push_back(std::move(func));
        }

        /// 执行管线 / Execute pipeline
        void execute(ospf::core::MetaModel& model) {
            for (auto& stage : stages_) {
                stage(model, PipelineStage::Registration);
                stage(model, PipelineStage::Validation);
                stage(model, PipelineStage::Transformation);
                stage(model, PipelineStage::Finalization);
            }
        }

        /// 阶段数量 / Stage count
        [[nodiscard]] std::size_t stage_count() const noexcept { return stages_.size(); }

    private:
        std::vector<StageFunc> stages_;
    };

}  // namespace ospf::framework
