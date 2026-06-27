#pragma once
/// Basic quadratic tetrad model /// 1:1 ∂‘”¶ Rust core/model/intermediate/basic_quadratic_tetrad_model.rs
#include <vector>
namespace ospf::core {
    struct BasicQuadraticTetradModel {
        std::vector<std::pair<std::size_t, double>> linear_objective;
        std::vector<std::tuple<std::size_t, std::size_t, double>> quadratic_objective;
    };
}