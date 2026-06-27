#pragma once
/// Quantities functional /// 1:1 ∂‘”¶ Rust quantities/functional.rs
#include <functional>
namespace ospf::quantities {
    template<typename V, typename U>
    struct QuantityFunctional {
        V value;
        U unit;
    };
}