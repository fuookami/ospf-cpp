#pragma once
/// Model constraint /// 1:1 ∂‘”¶ Rust core/model/constraint.rs
#include <string>
#include <vector>
namespace ospf::core {
    struct ModelConstraint {
        std::string name;
        std::vector<std::pair<std::size_t, double>> coefficients;
        double rhs = 0.0;
        enum class Sense : uint8_t { LessEqual, Equal, GreaterEqual };
        Sense sense = Sense::LessEqual;
    };
}