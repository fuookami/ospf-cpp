#pragma once

/// 错误基类 / Error base class
/// Error<C> 携带错误码 + 消息，C 默认为 ErrorCode。

#include <ospf/base/error/code.hpp>
#include <string>
#include <utility>

namespace ospf::base {

    /// 错误基类 / Error base class
    template<ErrorCodeTrait C = ErrorCode>
    class Error {
    public:
        C           code;     ///< 错误码 / Error code
        std::string message;  ///< 错误消息 / Error message

        Error() : code(C{}), message() {}
        Error(C code, std::string message)
            : code(code), message(std::move(message)) {}

        Error(const Error&) = default;
        Error(Error&&) noexcept = default;
        Error& operator=(const Error&) = default;
        Error& operator=(Error&&) noexcept = default;
        ~Error() = default;

        [[nodiscard]] bool operator==(const Error& other) const noexcept {
            return code == other.code && message == other.message;
        }

        [[nodiscard]] bool operator!=(const Error& other) const noexcept {
            return !(*this == other);
        }
    };

}  // namespace ospf::base
