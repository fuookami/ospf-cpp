#pragma once

/// 不等式 / Inequality
/// 表达式之间的不等式关系。

#include <ospf/math/symbol/expression.hpp>
#include <ostream>

namespace ospf::math::symbol {

    /// 比较运算符 / Comparison operator
    enum class ComparisonOp : std::uint8_t {
        Less,           ///< < (严格小于 / strictly less)
        LessEqual,      ///< <= (小于等于 / less or equal)
        Greater,        ///< > (严格大于 / strictly greater)
        GreaterEqual,   ///< >= (大于等于 / greater or equal)
        Equal,          ///< == (等于 / equal)
        NotEqual,       ///< != (不等于 / not equal)
    };

    /// 不等式 / Inequality: lhs op rhs
    class Inequality {
    public:
        Inequality(Expression lhs, ComparisonOp op, Expression rhs)
            : lhs_(std::move(lhs)), op_(op), rhs_(std::move(rhs)) {}

        // -- 工厂方法 / Factory methods ----------------------------------------

        [[nodiscard]] static Inequality less(Expression lhs, Expression rhs) {
            return Inequality(std::move(lhs), ComparisonOp::Less, std::move(rhs));
        }

        [[nodiscard]] static Inequality less_equal(Expression lhs, Expression rhs) {
            return Inequality(std::move(lhs), ComparisonOp::LessEqual, std::move(rhs));
        }

        [[nodiscard]] static Inequality greater(Expression lhs, Expression rhs) {
            return Inequality(std::move(lhs), ComparisonOp::Greater, std::move(rhs));
        }

        [[nodiscard]] static Inequality greater_equal(Expression lhs, Expression rhs) {
            return Inequality(std::move(lhs), ComparisonOp::GreaterEqual, std::move(rhs));
        }

        [[nodiscard]] static Inequality equal(Expression lhs, Expression rhs) {
            return Inequality(std::move(lhs), ComparisonOp::Equal, std::move(rhs));
        }

        // -- 访问 / Access ------------------------------------------------------

        [[nodiscard]] const Expression& lhs() const noexcept { return lhs_; }
        [[nodiscard]] ComparisonOp op() const noexcept { return op_; }
        [[nodiscard]] const Expression& rhs() const noexcept { return rhs_; }

        /// 转为标准形式（lhs - rhs op 0） / Convert to canonical form
        [[nodiscard]] Inequality canonical_form() const {
            return Inequality(lhs_ - rhs_, op_, Expression::zero());
        }

        /// 检查是否满足 / Check if satisfied
        [[nodiscard]] bool check(const std::map<Variable, double>& values) const {
            double l = lhs_.evaluate(values);
            double r = rhs_.evaluate(values);
            switch (op_) {
                case ComparisonOp::Less:         return l < r;
                case ComparisonOp::LessEqual:    return l <= r;
                case ComparisonOp::Greater:      return l > r;
                case ComparisonOp::GreaterEqual: return l >= r;
                case ComparisonOp::Equal:        return l == r;
                case ComparisonOp::NotEqual:     return l != r;
            }
            return false;
        }

        // -- 输出 / Output -----------------------------------------------------

        friend std::ostream& operator<<(std::ostream& os, const Inequality& ineq) {
            os << ineq.lhs_;
            switch (ineq.op_) {
                case ComparisonOp::Less:         os << " < "; break;
                case ComparisonOp::LessEqual:    os << " <= "; break;
                case ComparisonOp::Greater:      os << " > "; break;
                case ComparisonOp::GreaterEqual: os << " >= "; break;
                case ComparisonOp::Equal:        os << " == "; break;
                case ComparisonOp::NotEqual:     os << " != "; break;
            }
            return os << ineq.rhs_;
        }

    private:
        Expression lhs_;
        ComparisonOp op_;
        Expression rhs_;
    };

}  // namespace ospf::math::symbol
