#pragma once

/// Einsum trait — 张量运算 trait / Tensor operation trait
/// 1:1 对应 Rust einsum/einsum_trait.rs

#include <vector>
#include <string>

namespace ospf::multiarray::einsum {

    /// Einsum 运算 trait / Einsum operation trait
    template<typename T>
    class EinsumTrait {
    public:
        virtual ~EinsumTrait() = default;

        /// 执行 einsum 运算 / Execute einsum operation
        virtual std::vector<T> compute(
            const std::string& equation,
            const std::vector<const std::vector<T>*>& operands
        ) const = 0;
    };

}  // namespace ospf::multiarray::einsum
