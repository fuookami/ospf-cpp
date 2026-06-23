#pragma once

/// Rossler 混沌系统 / Rossler chaotic system

#include <array>

namespace ospf::math::chaotic {

    struct RosslerParams {
        double a = 0.2;
        double b = 0.2;
        double c = 5.7;
    };

    using RosslerState = std::array<double, 3>;

    [[nodiscard]] inline RosslerState rossler_step(
        const RosslerState& state, double dt, const RosslerParams& params = {})
    {
        double dx = -state[1] - state[2];
        double dy = state[0] + params.a * state[1];
        double dz = params.b + state[2] * (state[0] - params.c);

        return {
            state[0] + dx * dt,
            state[1] + dy * dt,
            state[2] + dz * dt
        };
    }

    [[nodiscard]] inline RosslerState rossler_iterate(
        RosslerState state, std::size_t steps, double dt, const RosslerParams& params = {})
    {
        for (std::size_t i = 0; i < steps; ++i) {
            state = rossler_step(state, dt, params);
        }
        return state;
    }

}  // namespace ospf::math::chaotic
