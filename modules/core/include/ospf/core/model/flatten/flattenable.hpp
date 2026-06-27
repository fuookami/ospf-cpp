#pragma once
/// Flattenable /// 1:1 ∂‘”¶ Rust core/model/flatten/flattenable.rs
namespace ospf::core {
    template<typename T>
    concept Flattenable = requires(T t) {
        { t.flatten() };
    };
}