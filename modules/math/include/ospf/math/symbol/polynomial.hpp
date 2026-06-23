#pragma once

/// 多项式 / Polynomial
/// 特定变量的多项式。

#include <ospf/math/symbol/expression.hpp>
#include <vector>

namespace ospf::math::symbol {

    /// 单变量多项式 / Single-variable polynomial
    class Polynomial {
    public:
        /// 从系数向量构造（从低次到高次） / Construct from coefficients (low to high degree)
        explicit Polynomial(std::vector<double> coefficients, Variable var)
            : coefficients_(std::move(coefficients)), variable_(std::move(var)) {}

        /// 零多项式 / Zero polynomial
        [[nodiscard]] static Polynomial zero(const Variable& var) {
            return Polynomial({0.0}, var);
        }

        /// 常数多项式 / Constant polynomial
        [[nodiscard]] static Polynomial constant(double value, const Variable& var) {
            return Polynomial({value}, var);
        }

        /// 线性多项式 / Linear polynomial (ax + b)
        [[nodiscard]] static Polynomial linear(double a, double b, const Variable& var) {
            return Polynomial({b, a}, var);
        }

        /// 二次多项式 / Quadratic polynomial (ax^2 + bx + c)
        [[nodiscard]] static Polynomial quadratic(double a, double b, double c, const Variable& var) {
            return Polynomial({c, b, a}, var);
        }

        // -- 访问 / Access ------------------------------------------------------

        [[nodiscard]] const std::vector<double>& coefficients() const noexcept { return coefficients_; }
        [[nodiscard]] const Variable& variable() const noexcept { return variable_; }

        /// 次数 / Degree
        [[nodiscard]] std::size_t degree() const noexcept {
            return coefficients_.empty() ? 0 : coefficients_.size() - 1;
        }

        // -- 运算 / Operations --------------------------------------------------

        /// 求值 / Evaluate (Horner's method)
        [[nodiscard]] double evaluate(double x) const {
            if (coefficients_.empty()) return 0.0;
            double result = coefficients_.back();
            for (std::size_t i = coefficients_.size() - 1; i > 0; --i) {
                result = result * x + coefficients_[i - 1];
            }
            return result;
        }

        /// 加法 / Addition
        [[nodiscard]] Polynomial operator+(const Polynomial& other) const {
            std::size_t max_size = std::max(coefficients_.size(), other.coefficients_.size());
            std::vector<double> result(max_size, 0.0);
            for (std::size_t i = 0; i < coefficients_.size(); ++i) {
                result[i] += coefficients_[i];
            }
            for (std::size_t i = 0; i < other.coefficients_.size(); ++i) {
                result[i] += other.coefficients_[i];
            }
            return Polynomial(std::move(result), variable_);
        }

        /// 标量乘法 / Scalar multiplication
        [[nodiscard]] Polynomial operator*(double scalar) const {
            std::vector<double> result = coefficients_;
            for (auto& c : result) c *= scalar;
            return Polynomial(std::move(result), variable_);
        }

        /// 转为表达式 / Convert to expression
        [[nodiscard]] Expression to_expression() const {
            Expression result;
            for (std::size_t i = 0; i < coefficients_.size(); ++i) {
                if (coefficients_[i] != 0.0) {
                    std::map<Variable, double> powers;
                    if (i > 0) powers[variable_] = static_cast<double>(i);
                    result = result + Expression(Monomial(coefficients_[i], std::move(powers)));
                }
            }
            return result;
        }

    private:
        std::vector<double> coefficients_;
        Variable variable_;
    };

}  // namespace ospf::math::symbol
