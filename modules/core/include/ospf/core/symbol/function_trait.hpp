#pragma once
/// Function trait /// 1:1 ∂‘”¶ Rust core/symbol/function_trait.rs
#include <string>
namespace ospf::core {
    template<typename T>
    concept FunctionTrait = requires(T t) {
        { t.name() } -> std::convertible_to<std::string>;
        { t.arity() } -> std::convertible_to<int>;
    };
}