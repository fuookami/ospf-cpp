#pragma once
/// Lazy context /// 1:1 ∂‘”¶ Rust/core/model/flatten/lazy_context.rs
#include <functional>
namespace ospf::core {
    struct LazyContext {
        std::function<double()> evaluator;
    };
}