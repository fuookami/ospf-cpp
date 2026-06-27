#pragma once
/// Symbol trait /// 1:1 ∂‘”¶ Rust core/symbol/symbol_trait.rs
#include <string>
namespace ospf::core {
    template<typename T>
    concept SymbolTrait = requires(T t) {
        { t.name() } -> std::convertible_to<std::string>;
    };
}