#pragma once

/// 延迟消息构造的错误 / Lazy error with deferred message construction
/// LazyError<C> 通过 std::function<std::string()> 延迟构造消息，避免不必要的格式化。

#include <ospf/base/error/error.hpp>
#include <functional>
#include <string>

namespace ospf::base {

    /// 延迟消息构造的错误 / Lazy error with deferred message construction
    template<ErrorCodeTrait C = ErrorCode>
    class LazyError : public Error<C> {
    public:
        /// 构造函数：接受延迟消息工厂。
        /// Constructor: accepts a lazy message factory.
        LazyError(C code, std::function<std::string()> message_factory)
            : Error<C>(code, ""), message_factory_(std::move(message_factory)) {}

        /// 获取消息（首次调用时构造，后续返回缓存）。
        /// Get message (constructed on first call, cached thereafter).
        [[nodiscard]] const std::string& message() const {
            if (!constructed_) {
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
                const_cast<LazyError*>(this)->Error<C>::message = message_factory_();
                const_cast<LazyError*>(this)->constructed_ = true;
            }
            return Error<C>::message;
        }

    private:
        std::function<std::string()> message_factory_;
        bool constructed_ = false;
    };

}  // namespace ospf::base
