#pragma once

/// Lorenz 混沌系统 / Lorenz chaotic system

#include <array>

namespace ospf::math::chaotic {

    /// Lorenz 系统参数 / Lorenz system parameters
    struct LorenzParams {
        double sigma = 10.0;
        double rho = 28.0;
        double beta = 8.0 / 3.0;
    };

    /// Lorenz 系统状态 / Lorenz system state
    using LorenzState = std::array<double, 3>;

    /// Lorenz 系统迭代 / Lorenz system iteration
    [[nodiscard]] inline LorenzState lorenz_step(
        const LorenzState& state, double dt, const LorenzParams& params = {})
    {
        double dx = params.sigma * (state[1] - state[0]);
        double dy = state[0] * (params.rho - state[2]) - state[1];
        double dz = state[0] * state[1] - params.beta * state[2];

        return {
            state[0] + dx * dt,
            state[1] + dy * dt,
            state[2] + dz * dt
        };
    }

    /// 迭代 N 步 / Iterate N steps
    [[nodiscard]] inline LorenzState lorenz_iterate(
        LorenzState state, std::size_t steps, double dt, const LorenzParams& params = {})
    {
        for (std::size_t i = 0; i < steps; ++i) {
            state = lorenz_step(state, dt, params);
        }
        return state;
    }

}  // namespace ospf::math::chaotic
