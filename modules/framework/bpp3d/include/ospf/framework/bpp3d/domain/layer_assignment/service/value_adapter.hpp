#pragma once
/// BPP3D domain/layer_assignment/service/value_adapter /// 1:1 对应 Rust bpp3d/domain/layer_assignment/service/value_adapter.rs
/// Bpp3dSolverValueAdapter trait + DefaultBpp3dSolverValueAdapter + ScaledBpp3dSolverValueAdapter

#include <cmath>
#include <string>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 求解器值适配器 / Solver value adapter
    // 对应 Rust Bpp3dSolverValueAdapter trait
    // ============================================================================

    /// 求解器值适配器 / Solver value adapter
    /// 将业务类型转换为求解器数值类型
    class Bpp3dSolverValueAdapter {
    public:
        virtual ~Bpp3dSolverValueAdapter() = default;
        [[nodiscard]] virtual const std::string& name() const = 0;
        [[nodiscard]] virtual double amount_to_solver(std::size_t value) const = 0;
        [[nodiscard]] virtual double length_to_solver(double value) const = 0;
        [[nodiscard]] virtual double weight_to_solver(double value) const = 0;
        [[nodiscard]] virtual double volume_to_solver(double value) const = 0;
        [[nodiscard]] virtual double depth_to_solver(double value) const = 0;
    };

    // ============================================================================
    // 默认求解器值适配器 / Default solver value adapter
    // 对应 Rust DefaultBpp3dSolverValueAdapter
    // ============================================================================

    /// 默认求解器值适配器 / Default solver value adapter
    /// 恒等映射（value → value）
    class DefaultBpp3dSolverValueAdapter : public Bpp3dSolverValueAdapter {
    public:
        [[nodiscard]] const std::string& name() const override {
            static const std::string n = "default";
            return n;
        }
        [[nodiscard]] double amount_to_solver(std::size_t value) const override { return static_cast<double>(value); }
        [[nodiscard]] double length_to_solver(double value) const override { return value; }
        [[nodiscard]] double weight_to_solver(double value) const override { return value; }
        [[nodiscard]] double volume_to_solver(double value) const override { return value; }
        [[nodiscard]] double depth_to_solver(double value) const override { return value; }
    };

    // ============================================================================
    // 缩放求解器值适配器 / Scaled solver value adapter
    // 对应 Rust ScaledBpp3dSolverValueAdapter
    // ============================================================================

    /// 缩放求解器值适配器 / Scaled solver value adapter
    /// 对每种物理量提供独立的缩放因子，用于求解器精度优化
    class ScaledBpp3dSolverValueAdapter : public Bpp3dSolverValueAdapter {
    public:
        double amount_scale = 1.0;
        double length_scale = 1.0;
        double weight_scale = 1.0;
        double volume_scale = 1.0;
        double depth_scale = 1.0;

        [[nodiscard]] const std::string& name() const override {
            static const std::string n = "scaled";
            return n;
        }

        /// 创建默认缩放（全1） / Create default scaled (all 1)
        [[nodiscard]] static ScaledBpp3dSolverValueAdapter default_adapter() {
            ScaledBpp3dSolverValueAdapter a;
            a.amount_scale = 1.0; a.length_scale = 1.0;
            a.weight_scale = 1.0; a.volume_scale = 1.0; a.depth_scale = 1.0;
            return a;
        }

        /// 创建十倍缩放 / Create ten-times scaled
        /// 对应 Rust ScaledBpp3dSolverValueAdapter::ten_times
        [[nodiscard]] static ScaledBpp3dSolverValueAdapter ten_times() {
            ScaledBpp3dSolverValueAdapter a;
            a.amount_scale = 10.0; a.length_scale = 10.0;
            a.weight_scale = 10.0; a.volume_scale = 10.0; a.depth_scale = 10.0;
            return a;
        }

        [[nodiscard]] double amount_to_solver(std::size_t value) const override {
            return static_cast<double>(value) * amount_scale;
        }
        [[nodiscard]] double length_to_solver(double value) const override { return value * length_scale; }
        [[nodiscard]] double weight_to_solver(double value) const override { return value * weight_scale; }
        [[nodiscard]] double volume_to_solver(double value) const override { return value * volume_scale; }
        [[nodiscard]] double depth_to_solver(double value) const override { return value * depth_scale; }
    };

}  // namespace ospf::framework::bpp3d
