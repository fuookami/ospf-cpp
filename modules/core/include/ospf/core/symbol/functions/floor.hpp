#pragma once
/// 符号函数: floor / Symbol function: floor
/// 1:1 对应 Rust core/symbol/functions/floor.rs

#include <ospf/core/symbol/symbol_trait.hpp>
#include <string>

namespace ospf::core {

    /// floor 符号函数 / floor symbol function
    /// 优化建模用符号函数，非数学函数。
    /// Symbol function for optimization modeling, not a mathematical function.
    template<typename V = double>
    class FloorFunction {
    public:
        [[nodiscard]] static const char* name() noexcept { return "floor"; }

        // TODO: 填充实现（对照 Rust floor.rs）
        // TODO: Fill implementation (match Rust floor.rs)
    };

}  // namespace ospf::core
