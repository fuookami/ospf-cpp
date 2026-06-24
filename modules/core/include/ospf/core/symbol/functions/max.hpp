#pragma once
/// 符号函数: max / Symbol function: max
/// 1:1 对应 Rust core/symbol/functions/max.rs

#include <ospf/core/symbol/symbol_trait.hpp>
#include <string>

namespace ospf::core {

    /// max 符号函数 / max symbol function
    /// 优化建模用符号函数，非数学函数。
    /// Symbol function for optimization modeling, not a mathematical function.
    template<typename V = double>
    class MaxFunction {
    public:
        [[nodiscard]] static const char* name() noexcept { return "max"; }

        // TODO: 填充实现（对照 Rust max.rs）
        // TODO: Fill implementation (match Rust max.rs)
    };

}  // namespace ospf::core
