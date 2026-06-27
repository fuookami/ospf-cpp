#pragma once
/// Unit concept /// 1:1 ∂‘”¶ Rust quantities/unit/concept.rs
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    template<typename U>
    concept UnitLike = requires {
        { U::dimension } -> std::convertible_to<Dimension>;
        { U::to_si_factor } -> std::convertible_to<double>;
        { U::name } -> std::convertible_to<std::string>;
    };
}