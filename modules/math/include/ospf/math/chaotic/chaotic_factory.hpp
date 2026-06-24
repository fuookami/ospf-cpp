#pragma once

/// 混沌系统模板工厂 / Chaotic system template factory
/// 用于生成 1:1 对应 Rust 的 88 个混沌系统头文件。
/// Used to generate 1:1 corresponding Rust chaotic system headers.

#include <array>
#include <cstddef>
#include <cmath>

namespace ospf::math::chaotic {

    /// 3D 点 / 3D point
    template<typename S = double>
    using Point3 = std::array<S, 3>;

    /// 2D 点 / 2D point
    template<typename S = double>
    using Point2 = std::array<S, 2>;

    /// 3D 混沌系统模板 / 3D chaotic system template
    /// Params 必须提供 dt 系数计算方法 / Params must provide dt coefficient computation
    template<typename Params, const char* Name>
    struct ChaoticSystem3D {
        static constexpr const char* name = Name;
        Params params;

        template<typename S = double>
        [[nodiscard]] Point3<S> step(const Point3<S>& state, S dt) const {
            auto deriv = params.derivatives(state);
            return {
                state[0] + deriv[0] * dt,
                state[1] + deriv[1] * dt,
                state[2] + deriv[2] * dt
            };
        }

        template<typename S = double>
        [[nodiscard]] Point3<S> iterate(Point3<S> state, std::size_t steps, S dt) const {
            for (std::size_t i = 0; i < steps; ++i) {
                state = step(state, dt);
            }
            return state;
        }
    };

    /// 2D 混沌系统模板 / 2D chaotic system template
    template<typename Params, const char* Name>
    struct ChaoticSystem2D {
        static constexpr const char* name = Name;
        Params params;

        template<typename S = double>
        [[nodiscard]] Point2<S> step(const Point2<S>& state, S dt) const {
            auto deriv = params.derivatives(state);
            return {
                state[0] + deriv[0] * dt,
                state[1] + deriv[1] * dt
            };
        }

        template<typename S = double>
        [[nodiscard]] Point2<S> iterate(Point2<S> state, std::size_t steps, S dt) const {
            for (std::size_t i = 0; i < steps; ++i) {
                state = step(state, dt);
            }
            return state;
        }
    };

    /// 标量映射模板 / Scalar map template
    template<typename Params, const char* Name>
    struct ScalarMap {
        static constexpr const char* name = Name;
        Params params;

        template<typename S = double>
        [[nodiscard]] S iterate(S x, std::size_t steps) const {
            for (std::size_t i = 0; i < steps; ++i) {
                x = params.map(x);
            }
            return x;
        }
    };

}  // namespace ospf::math::chaotic
