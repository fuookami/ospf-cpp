#pragma once
/// Multi-objective model /// 1:1 ∂‘”¶ Rust/core/model/callback/multi_objective_model.rs
#include <vector>
#include <string>
namespace ospf::core {
    struct MultiObjectiveModel {
        std::vector<std::string> objective_names;
        std::vector<double> weights;
    };
}