#pragma once

/// Benders 分解 / Benders decomposition

#include <ospf/core/solver.hpp>
#include <functional>
#include <string>
#include <vector>

namespace ospf::framework {

    /// Benders 分解阶段 / Benders decomposition stage
    enum class BendersStage : std::uint8_t {
        MasterProblem,   ///< 主问题
        SubProblem,      ///< 子问题
        AddCut,          ///< 添加割平面
        Convergence,     ///< 收敛检查
        Finalize,        ///< 完成
    };

    /// Benders 割平面 / Benders cut
    struct BendersCut {
        std::string name;
        std::vector<double> coefficients;
        double rhs = 0.0;
    };

    /// Benders 分解求解器 / Benders decomposition solver
    class BendersSolver {
    public:
        using StageCallback = std::function<void(BendersStage)>;

        void on_stage(StageCallback callback) {
            callback_ = std::move(callback);
        }

        /// 求解主问题 / Solve master problem
        ospf::core::SolveResult solve_master(const ospf::core::MetaModel& /*model*/) {
            notify(BendersStage::MasterProblem);
            // stub 实现
            ospf::core::SolveResult result;
            result.status = ospf::core::SolveStatus::Optimal;
            result.objective_value = 0.0;
            return result;
        }

        /// 求解子问题 / Solve sub problem
        ospf::core::SolveResult solve_sub(const ospf::core::MetaModel& /*model*/) {
            notify(BendersStage::SubProblem);
            ospf::core::SolveResult result;
            result.status = ospf::core::SolveStatus::Optimal;
            result.objective_value = 0.0;
            return result;
        }

        /// 添加割平面 / Add cut
        void add_cut(BendersCut cut) {
            cuts_.push_back(std::move(cut));
            notify(BendersStage::AddCut);
        }

        /// 检查收敛 / Check convergence
        bool check_convergence(double gap, double tolerance = 1e-6) {
            notify(BendersStage::Convergence);
            return gap <= tolerance;
        }

        /// 完成 / Finalize
        void finalize() {
            notify(BendersStage::Finalize);
        }

        [[nodiscard]] const std::vector<BendersCut>& cuts() const noexcept { return cuts_; }

    private:
        void notify(BendersStage stage) {
            if (callback_) callback_(stage);
        }

        std::vector<BendersCut> cuts_;
        StageCallback callback_;
    };

}  // namespace ospf::framework
