#pragma once
/// 符号函数: if_in / Symbol function: if_in
/// 1:1 对应 Rust core/symbol/functions/if_in.rs

#include <ospf/core/symbol/symbol_trait.hpp>
#include <string>

namespace ospf::core {

    /// if_in 符号函数 / if_in symbol function
    /// 优化建模用符号函数，非数学函数。
    /// Symbol function for optimization modeling, not a mathematical function.
    template<typename V = double>
    class If_inFunction {
    public:
        [[nodiscard]] static const char* name() noexcept { return "if_in"; }

        // TODO: 填充实现（对照 Rust if_in.rs）
        // TODO: Fill implementation (match Rust if_in.rs)
    };

}  // namespace ospf::core
