#pragma once
/// 符号函数: first / Symbol function: first
/// 1:1 对应 Rust core/symbol/functions/first.rs

#include <ospf/core/symbol/symbol_trait.hpp>
#include <string>

namespace ospf::core {

    /// first 符号函数 / first symbol function
    /// 优化建模用符号函数，非数学函数。
    /// Symbol function for optimization modeling, not a mathematical function.
    template<typename V = double>
    class FirstFunction {
    public:
        [[nodiscard]] static const char* name() noexcept { return "first"; }

        // TODO: 填充实现（对照 Rust first.rs）
        // TODO: Fill implementation (match Rust first.rs)
    };

}  // namespace ospf::core
