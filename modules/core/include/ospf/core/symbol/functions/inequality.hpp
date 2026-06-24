#pragma once
/// 符号函数: inequality / Symbol function: inequality
/// 1:1 对应 Rust core/symbol/functions/inequality.rs

#include <ospf/core/symbol/symbol_trait.hpp>
#include <string>

namespace ospf::core {

    /// inequality 符号函数 / inequality symbol function
    /// 优化建模用符号函数，非数学函数。
    /// Symbol function for optimization modeling, not a mathematical function.
    template<typename V = double>
    class InequalityFunction {
    public:
        [[nodiscard]] static const char* name() noexcept { return "inequality"; }

        // TODO: 填充实现（对照 Rust inequality.rs）
        // TODO: Fill implementation (match Rust inequality.rs)
    };

}  // namespace ospf::core
