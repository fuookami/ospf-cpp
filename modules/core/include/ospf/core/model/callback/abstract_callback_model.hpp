#pragma once
/// Abstract callback model /// 1:1 ∂‘”¶ Rust/core/model/callback/abstract_callback_model.rs
#include <string>
namespace ospf::core {
    struct CallbackModel {
        std::string name;
        virtual ~CallbackModel() = default;
    };
}