#pragma once
/// 符号类别 / Symbol category /// 1:1 对应 Rust symbol/category.rs
namespace ospf::math::symbol {
    enum class SymbolCategory : std::uint8_t { Variable = 0, Constant = 1, Function = 2 };
}
