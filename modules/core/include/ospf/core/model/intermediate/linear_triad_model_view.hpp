#pragma once
/// Linear triad model view /// 1:1 ∂‘”¶ Rust/core/model/intermediate/linear_triad_model_view.rs
#include <vector>
namespace ospf::core {
    struct LinearTriadModelView {
        std::size_t variable_count = 0;
        std::size_t constraint_count = 0;
    };
}