#pragma once

/// 单项式 / Monomial
/// 系数 * 变量1^指数1 * 变量2^指数2 * ...

#include <ospf/math/symbol/variable.hpp>
#include <cmath>
#include <map>
#include <ostream>

namespace ospf::math::symbol {

    /// 单项式 / Monomial: coefficient * product of variable powers
    class Monomial {
    public:
        /// 构造：常数项 / Construct: constant
        explicit Monomial(double coefficient = 0.0)
            : coefficient_(coefficient) {}

        /// 构造：系数 * 变量 / Construct: coefficient * variable
        Monomial(double coefficient, const Variable& var, double exponent = 1.0)
            : coefficient_(coefficient) {
            if (exponent != 0.0) {
                powers_[var] = exponent;
            }
        }

        /// 构造：系数 * 变量幂次表 / Construct: coefficient * variable powers
        Monomial(double coefficient, std::map<Variable, double> powers)
            : coefficient_(coefficient), powers_(std::move(powers)) {}

        // -- 工厂方法 / Factory methods ----------------------------------------

        /// 零单项式 / Zero monomial
        [[nodiscard]] static Monomial zero() { return Monomial(0.0); }

        /// 常数单项式 / Constant monomial
        [[nodiscard]] static Monomial constant(double value) { return Monomial(value); }

        /// 变量单项式（系数为 1） / Variable monomial (coefficient = 1)
        [[nodiscard]] static Monomial variable(const Variable& var) {
            return Monomial(1.0, var);
        }

        // -- 访问 / Access ------------------------------------------------------

        [[nodiscard]] double coefficient() const noexcept { return coefficient_; }
        [[nodiscard]] const std::map<Variable, double>& powers() const noexcept { return powers_; }

        /// 是否为常数 / Is constant (no variables)
        [[nodiscard]] bool is_constant() const noexcept { return powers_.empty(); }

        /// 是否为零 / Is zero
        [[nodiscard]] bool is_zero() const noexcept { return coefficient_ == 0.0; }

        /// 总次数 / Total degree
        [[nodiscard]] double total_degree() const noexcept {
            double deg = 0.0;
            for (const auto& [_, exp] : powers_) {
                deg += exp;
            }
            return deg;
        }

        /// 指定变量的次数 / Degree of a specific variable
        [[nodiscard]] double degree_of(const Variable& var) const noexcept {
            auto it = powers_.find(var);
            return it != powers_.end() ? it->second : 0.0;
        }

        // -- 运算 / Operations --------------------------------------------------

        /// 求值 / Evaluate
        [[nodiscard]] double evaluate(const std::map<Variable, double>& values) const {
            double result = coefficient_;
            for (const auto& [var, exp] : powers_) {
                auto it = values.find(var);
                if (it != values.end()) {
                    result *= std::pow(it->second, exp);
                }
            }
            return result;
        }

        /// 乘法 / Multiplication
        [[nodiscard]] Monomial operator*(const Monomial& other) const {
            std::map<Variable, double> new_powers = powers_;
            for (const auto& [var, exp] : other.powers_) {
                new_powers[var] += exp;
            }
            return Monomial(coefficient_ * other.coefficient_, std::move(new_powers));
        }

        /// 标量乘法 / Scalar multiplication
        [[nodiscard]] Monomial operator*(double scalar) const {
            return Monomial(coefficient_ * scalar, powers_);
        }

        [[nodiscard]] friend Monomial operator*(double scalar, const Monomial& m) {
            return m * scalar;
        }

        /// 是否同类项 / Is like term (same variables and powers)
        [[nodiscard]] bool is_like_term(const Monomial& other) const noexcept {
            return powers_ == other.powers_;
        }

        /// 合并同类项 / Combine like terms
        [[nodiscard]] Monomial combine(const Monomial& other) const {
            return Monomial(coefficient_ + other.coefficient_, powers_);
        }

        // -- 比较 / Comparison -------------------------------------------------

        [[nodiscard]] bool operator==(const Monomial& other) const noexcept {
            return coefficient_ == other.coefficient_ && powers_ == other.powers_;
        }

        [[nodiscard]] bool operator<(const Monomial& other) const noexcept {
            if (powers_ != other.powers_) return powers_ < other.powers_;
            return coefficient_ < other.coefficient_;
        }

        // -- 输出 / Output -----------------------------------------------------

        friend std::ostream& operator<<(std::ostream& os, const Monomial& m) {
            if (m.is_zero()) return os << "0";
            if (m.coefficient_ != 1.0 || m.powers_.empty()) {
                os << m.coefficient_;
            }
            for (const auto& [var, exp] : m.powers_) {
                os << "*" << var;
                if (exp != 1.0) os << "^" << exp;
            }
            return os;
        }

    private:
        double coefficient_;
        std::map<Variable, double> powers_;
    };

}  // namespace ospf::math::symbol
