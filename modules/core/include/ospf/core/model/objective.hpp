#pragma once
/// Model objective /// 1:1 ∂‘”¶ Rust/core/model/objective.rs
#include <string>
#include <vector>
namespace ospf::core {
    struct ModelObjective {
        std::string name;
        enum class Sense : uint8_t { Minimize, Maximize };
        Sense sense = Sense::Minimize;
        std::vector<std::pair<std::size_t, double>> coefficients;
    };
}