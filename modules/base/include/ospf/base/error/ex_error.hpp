#pragma once

/// 带关联值的错误 / Error with associated value
/// ExError<C, V> 继承 Error<C>，额外携带关联值 V。

#include <ospf/base/error/error.hpp>
#include <utility>

namespace ospf::base {

    /// 带关联值的错误 / Error with associated value
    template<ErrorCodeTrait C = ErrorCode, typename V = void>
    class ExError : public Error<C> {
    public:
        V value;  ///< 关联值 / Associated value

        ExError(C code, std::string message, V value)
            : Error<C>(code, std::move(message)), value(std::move(value)) {}

        ExError(const ExError&) = default;
        ExError(ExError&&) noexcept = default;
        ExError& operator=(const ExError&) = default;
        ExError& operator=(ExError&&) noexcept = default;
        ~ExError() = default;
    };

    /// 特化：无关联值时退化为 Error<C>。
    /// Specialization: when V=void, degenerates to Error<C>.
    template<ErrorCodeTrait C>
    class ExError<C, void> : public Error<C> {
    public:
        using Error<C>::Error;
    };

}  // namespace ospf::base
