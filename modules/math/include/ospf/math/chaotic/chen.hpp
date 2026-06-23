#pragma once

/// Chen 混沌系统 / Chen chaotic system

#include <array>

namespace ospf::math::chaotic {

    struct ChenParams {
        double a = 35.0;
        double b = 3.0;
        double c = 28.0;
    };

    using ChenState = std::array<double, 3>;

    [[nodiscard]] inline ChenState chen_step(
        const ChenState& state, double dt, const ChenParams& params = {})
    {
        double dx = params.a * (state[1] - state[0]);
        double dy = (params.c - params.a) * state[0] - state[0] * state[2] + params.c * state[1];
        double dz = state[0] * state[1] - params.b * state[2];

        return {
            state[0] + dx * dt,
            state[1] + dy * dt,
            state[2] + dz * dt
        };
    }

    [[nodiscard]] inline ChenState chen_iterate(
        ChenState state, std::size_t steps, double dt, const ChenParams& params = {})
    {
        for (std::size_t i = 0; i < steps; ++i) {
            state = chen_step(state, dt, params);
        }
        return state;
    }

}  // namespace ospf::math::chaotic
