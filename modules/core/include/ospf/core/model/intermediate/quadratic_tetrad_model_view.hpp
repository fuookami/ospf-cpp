#pragma once
/// Quadratic tetrad model view /// 1:1 ∂‘”¶ Rust/core/model/intermediate/quadratic_tetrad_model_view.rs
#include <vector>
namespace ospf::core {
    struct QuadraticTetradModelView {
        std::size_t variable_count = 0;
        std::size_t constraint_count = 0;
    };
}