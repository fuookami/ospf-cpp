#pragma once

/// 变量系统 / Variable system
/// 变量是模型中的决策变量。

#include <ospf/core/symbol.hpp>
#include <cstdint>
#include <string>
#include <optional>
#include <limits>

namespace ospf::core {

    /// 变量类型 / Variable type
    enum class VariableType : std::uint8_t {
        Continuous,  ///< 连续变量 / Continuous
        Integer,     ///< 整数变量 / Integer
        Binary,      ///< 二元变量 / Binary (0 or 1)
    };

    /// 变量边界 / Variable bounds
    struct VariableBounds {
        double lower = 0.0;
        double upper = std::numeric_limits<double>::infinity();

        [[nodiscard]] bool is_valid() const noexcept { return lower <= upper; }
    };

    /// 变量 / Variable: decision variable in the model
    class Variable {
    public:
        Variable(Symbol symbol, VariableType type = VariableType::Continuous,
                 VariableBounds bounds = {}, std::optional<double> initial = std::nullopt)
            : symbol_(std::move(symbol)), type_(type), bounds_(bounds), initial_(initial) {}

        [[nodiscard]] const Symbol& symbol() const noexcept { return symbol_; }
        [[nodiscard]] const std::string& name() const noexcept { return symbol_.name(); }
        [[nodiscard]] VariableType type() const noexcept { return type_; }
        [[nodiscard]] const VariableBounds& bounds() const noexcept { return bounds_; }
        [[nodiscard]] std::optional<double> initial() const noexcept { return initial_; }

        [[nodiscard]] bool operator==(const Variable& other) const noexcept {
            return symbol_ == other.symbol_;
        }

    private:
        Symbol symbol_;
        VariableType type_;
        VariableBounds bounds_;
        std::optional<double> initial_;
    };

    /// 变量构建器 / Variable builder
    class VariableBuilder {
    public:
        explicit VariableBuilder(SymbolRegistry& registry = SymbolRegistry::instance())
            : registry_(registry) {}

        VariableBuilder& name(const std::string& n) { name_ = n; return *this; }
        VariableBuilder& type(VariableType t) { type_ = t; return *this; }
        VariableBuilder& lower(double l) { bounds_.lower = l; return *this; }
        VariableBuilder& upper(double u) { bounds_.upper = u; return *this; }
        VariableBuilder& bounds(double l, double u) { bounds_ = {l, u}; return *this; }
        VariableBuilder& initial(double v) { initial_ = v; return *this; }

        [[nodiscard]] Variable build() {
            auto sym = registry_.create_symbol(name_, SymbolType::Variable);
            return Variable(sym, type_, bounds_, initial_);
        }

    private:
        SymbolRegistry& registry_;
        std::string name_;
        VariableType type_ = VariableType::Continuous;
        VariableBounds bounds_ = {0.0, std::numeric_limits<double>::infinity()};
        std::optional<double> initial_;
    };

}  // namespace ospf::core
