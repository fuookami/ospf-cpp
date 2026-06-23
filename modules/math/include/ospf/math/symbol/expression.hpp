#pragma once

/// 符号表达式 / Symbolic expression
/// 多项式表达式（单项式的和）。

#include <ospf/math/symbol/monomial.hpp>
#include <algorithm>
#include <map>
#include <vector>

namespace ospf::math::symbol {

    /// 符号表达式（单项式的和） / Symbolic expression (sum of monomials)
    class Expression {
    public:
        Expression() = default;

        /// 从单项式构造 / Construct from monomial
        Expression(Monomial mono) {  // NOLINT(google-explicit-constructor)
            if (!mono.is_zero()) {
                terms_.push_back(std::move(mono));
            }
        }

        /// 从常数构造 / Construct from constant
        Expression(double value) {  // NOLINT(google-explicit-constructor)
            if (value != 0.0) {
                terms_.push_back(Monomial::constant(value));
            }
        }

        /// 从变量构造 / Construct from variable
        Expression(Variable var) {  // NOLINT(google-explicit-constructor)
            terms_.push_back(Monomial::variable(std::move(var)));
        }

        // -- 工厂方法 / Factory methods ----------------------------------------

        [[nodiscard]] static Expression zero() { return Expression(); }

        [[nodiscard]] static Expression constant(double value) {
            return Expression(value);
        }

        [[nodiscard]] static Expression variable(const Variable& var) {
            return Expression(Monomial::variable(var));
        }

        // -- 访问 / Access ------------------------------------------------------

        [[nodiscard]] const std::vector<Monomial>& terms() const noexcept { return terms_; }
        [[nodiscard]] bool is_zero() const noexcept { return terms_.empty(); }
        [[nodiscard]] bool is_constant() const noexcept {
            return terms_.empty() || (terms_.size() == 1 && terms_[0].is_constant());
        }

        // -- 运算 / Operations --------------------------------------------------

        /// 求值 / Evaluate
        [[nodiscard]] double evaluate(const std::map<Variable, double>& values) const {
            double result = 0.0;
            for (const auto& term : terms_) {
                result += term.evaluate(values);
            }
            return result;
        }

        /// 加法 / Addition
        [[nodiscard]] Expression operator+(const Expression& other) const {
            Expression result = *this;
            for (const auto& term : other.terms_) {
                result.add_term(term);
            }
            return result;
        }

        /// 减法 / Subtraction
        [[nodiscard]] Expression operator-(const Expression& other) const {
            Expression result = *this;
            for (const auto& term : other.terms_) {
                result.add_term(Monomial(-term.coefficient(), term.powers()));
            }
            return result;
        }

        /// 取负 / Negation
        [[nodiscard]] Expression operator-() const {
            Expression result;
            for (const auto& term : terms_) {
                result.terms_.push_back(Monomial(-term.coefficient(), term.powers()));
            }
            return result;
        }

        /// 标量乘法 / Scalar multiplication
        [[nodiscard]] Expression operator*(double scalar) const {
            Expression result;
            for (const auto& term : terms_) {
                result.terms_.push_back(term * scalar);
            }
            return result;
        }

        [[nodiscard]] friend Expression operator*(double scalar, const Expression& e) {
            return e * scalar;
        }

        /// 表达式乘法 / Expression multiplication
        [[nodiscard]] Expression operator*(const Expression& other) const {
            Expression result;
            for (const auto& a : terms_) {
                for (const auto& b : other.terms_) {
                    result.add_term(a * b);
                }
            }
            return result;
        }

        /// 化简（合并同类项） / Simplify (combine like terms)
        [[nodiscard]] Expression simplified() const {
            Expression result;
            for (const auto& term : terms_) {
                result.add_term(term);
            }
            return result;
        }

        // -- 比较 / Comparison -------------------------------------------------

        [[nodiscard]] bool operator==(const Expression& other) const noexcept {
            return terms_ == other.terms_;
        }

        // -- 输出 / Output -----------------------------------------------------

        friend std::ostream& operator<<(std::ostream& os, const Expression& e) {
            if (e.is_zero()) return os << "0";
            bool first = true;
            for (const auto& term : e.terms_) {
                if (!first && term.coefficient() > 0) os << "+";
                os << term;
                first = false;
            }
            return os;
        }

    private:
        void add_term(const Monomial& term) {
            // 查找同类项 / Find like term
            for (auto& existing : terms_) {
                if (existing.is_like_term(term)) {
                    existing = existing.combine(term);
                    if (existing.is_zero()) {
                        std::erase(terms_, existing);
                    }
                    return;
                }
            }
            if (!term.is_zero()) {
                terms_.push_back(term);
            }
        }

        std::vector<Monomial> terms_;
    };

}  // namespace ospf::math::symbol
