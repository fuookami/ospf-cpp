#pragma once

/// Henon 映射 / Henon map

#include <array>

namespace ospf::math::chaotic {

    struct HenonParams {
        double a = 1.4;
        double b = 0.3;
    };

    using HenonState = std::array<double, 2>;

    [[nodiscard]] inline HenonState henon_map(const HenonState& state, const HenonParams& params = {}) {
        return {
            1.0 - params.a * state[0] * state[0] + state[1],
            params.b * state[0]
        };
    }

    [[nodiscard]] inline HenonState henon_iterate(
        HenonState state, std::size_t steps, const HenonParams& params = {})
    {
        for (std::size_t i = 0; i < steps; ++i) {
            state = henon_map(state, params);
        }
        return state;
    }

}  // namespace ospf::math::chaotic
