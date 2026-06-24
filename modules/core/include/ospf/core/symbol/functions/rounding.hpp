#pragma once
/// 符号函数: rounding / Symbol function: rounding
/// 1:1 对应 Rust core/symbol/functions/rounding.rs

#include <ospf/core/symbol/symbol_trait.hpp>
#include <string>

namespace ospf::core {

    /// rounding 符号函数 / rounding symbol function
    /// 优化建模用符号函数，非数学函数。
    /// Symbol function for optimization modeling, not a mathematical function.
    template<typename V = double>
    class RoundingFunction {
    public:
        [[nodiscard]] static const char* name() noexcept { return "rounding"; }

        // TODO: 填充实现（对照 Rust rounding.rs）
        // TODO: Fill implementation (match Rust rounding.rs)
    };

}  // namespace ospf::core
