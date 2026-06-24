#pragma once

/// multiarray 错误类型 / Multiarray error types
/// 1:1 对应 Rust error.rs

#include <ospf/base/error.hpp>
#include <string>

namespace ospf::multiarray {

    /// 维度不匹配错误 / Dimension mismatching error
    struct DimensionMismatchingError {
        std::size_t expected;
        std::size_t actual;
        std::string message;

        [[nodiscard]] auto code() const { return ospf::base::ErrorCode::IllegalArgument; }
    };

    /// 索引计算错误 / Index calculation error
    struct IndexCalculationError {
        std::string message;

        [[nodiscard]] auto code() const { return ospf::base::ErrorCode::IllegalArgument; }
    };

    /// 超出形状范围错误 / Out of shape error
    struct OutOfShapeError {
        std::string message;

        [[nodiscard]] auto code() const { return ospf::base::ErrorCode::IllegalArgument; }
    };

}  // namespace ospf::multiarray
