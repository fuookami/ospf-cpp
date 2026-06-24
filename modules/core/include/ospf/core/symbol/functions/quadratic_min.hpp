#pragma once
/// 符号函数: quadratic_min / Symbol function: quadratic_min
/// 1:1 对应 Rust core/symbol/functions/quadratic_min.rs

#include <ospf/core/symbol/symbol_trait.hpp>
#include <string>

namespace ospf::core {

    /// quadratic_min 符号函数 / quadratic_min symbol function
    /// 优化建模用符号函数，非数学函数。
    /// Symbol function for optimization modeling, not a mathematical function.
    template<typename V = double>
    class Quadratic_minFunction {
    public:
        [[nodiscard]] static const char* name() noexcept { return "quadratic_min"; }

        // TODO: 填充实现（对照 Rust quadratic_min.rs）
        // TODO: Fill implementation (match Rust quadratic_min.rs)
    };

}  // namespace ospf::core
