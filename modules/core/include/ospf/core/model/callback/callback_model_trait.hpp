#pragma once
/// Callback model trait /// 1:1 ∂‘”¶ Rust/core/model/callback/callback_model_trait.rs
namespace ospf::core {
    template<typename T>
    concept CallbackModelTrait = requires(T t) {
        { t.name() } -> std::convertible_to<std::string>;
    };
}