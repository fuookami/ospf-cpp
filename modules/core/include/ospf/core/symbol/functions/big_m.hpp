#pragma once
/// 符号函数: big_m / Symbol function: big_m
/// 1:1 对应 Rust core/symbol/functions/big_m.rs

#include <ospf/core/symbol/symbol_trait.hpp>
#include <string>

namespace ospf::core {

    /// big_m 符号函数 / big_m symbol function
    /// 优化建模用符号函数，非数学函数。
    /// Symbol function for optimization modeling, not a mathematical function.
    template<typename V = double>
    class Big_mFunction {
    public:
        [[nodiscard]] static const char* name() noexcept { return "big_m"; }

        // TODO: 填充实现（对照 Rust big_m.rs）
        // TODO: Fill implementation (match Rust big_m.rs)
    };

}  // namespace ospf::core
