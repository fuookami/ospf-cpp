#pragma once

/// 张量表达式 / Tensor expression
/// 1:1 对应 Rust einsum/tensor_expr.rs

#include <vector>
#include <string>

namespace ospf::multiarray::einsum {

    /// 张量表达式节点 / Tensor expression node
    struct TensorExpr {
        enum class Type { Variable, Sum, Product };

        Type type;
        std::string name;
        std::vector<TensorExpr> children;

        [[nodiscard]] static TensorExpr variable(std::string name) {
            return { Type::Variable, std::move(name), {} };
        }

        [[nodiscard]] static TensorExpr sum(TensorExpr child) {
            return { Type::Sum, {}, {std::move(child)} };
        }

        [[nodiscard]] static TensorExpr product(TensorExpr left, TensorExpr right) {
            return { Type::Product, {}, {std::move(left), std::move(right)} };
        }
    };

}  // namespace ospf::multiarray::einsum
