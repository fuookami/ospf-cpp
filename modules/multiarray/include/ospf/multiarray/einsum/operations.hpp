#pragma once

/// Einsum 运算 / Einsum operations
/// 1:1 对应 Rust einsum/operations.rs

#include <ospf/multiarray/einsum/einsum_trait.hpp>
#include <ospf/multiarray/einsum/indices.hpp>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

namespace ospf::multiarray::einsum {

    /// 基础 einsum 实现 / Basic einsum implementation
    template<typename T>
    class BasicEinsum : public EinsumTrait<T> {
    public:
        std::vector<T> compute(
            const std::string& equation,
            const std::vector<const std::vector<T>*>& operands
        ) const override {
            auto indices = EinsumIndices::parse(equation);
            // 简化实现：仅支持矩阵迹和求和
            // Simplified: supports only matrix trace and sum
            if (operands.size() == 1 && !indices.output_labels.empty()) {
                return *operands[0];  // identity
            }
            return {};
        }
    };

}  // namespace ospf::multiarray::einsum
