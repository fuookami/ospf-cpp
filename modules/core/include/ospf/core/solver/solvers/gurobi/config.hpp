#pragma once
/// Gurobi 求解器配置 / Gurobi solver configuration
/// 1:1 对应 Rust core/solver/solvers/gurobi/config.rs

#include <optional>
#include <cstdint>

namespace ospf::core {

    /// Gurobi 数值诊断配置文件 / Gurobi numeric profile
    enum class GurobiNumericProfile : std::uint8_t {
        Performance = 0,  ///< 性能优先 / Performance first
        Balanced = 1,     ///< 平衡 / Balanced
        Robust = 2,       ///< 鲁棒性优先 / Robustness first
    };

    /// Gurobi 求解器配置 / Gurobi solver configuration
    struct GurobiConfig {
        /// 时间限制（秒）/ Time limit (seconds)
        double time_limit = 3600.0;

        /// MIP Gap 容差 / MIP gap tolerance
        double mip_gap = 1e-4;

        /// 系数零值容差 / Coefficient zero tolerance
        double coefficient_zero_tolerance = 1e-13;

        /// 是否启用数值诊断 / Enable numeric diagnostics
        bool enable_numeric_diagnostics = true;

        /// 数值焦点 / Numeric focus
        std::optional<int> numeric_focus;

        /// 缩放标志 / Scale flag
        std::optional<int> scale_flag;

        /// 输出抑制 / Suppress output
        bool suppress_output = true;

        /// 线程数 / Thread count (0 = automatic)
        int threads = 0;
    };

}  // namespace ospf::core
