#pragma once
/// 符号函数: abs / Symbol function: abs
/// 1:1 对应 Rust core/symbol/functions/abs.rs

#include <ospf/core/symbol/symbol_trait.hpp>
#include <string>

namespace ospf::core {

    /// abs 符号函数 / abs symbol function
    /// 优化建模用符号函数，非数学函数。
    /// Symbol function for optimization modeling, not a mathematical function.
    template<typename V = double>
    class AbsFunction {
    public:
        [[nodiscard]] static const char* name() noexcept { return "abs"; }

        // TODO: 填充实现（对照 Rust abs.rs）
        // TODO: Fill implementation (match Rust abs.rs)
    };

}  // namespace ospf::core
