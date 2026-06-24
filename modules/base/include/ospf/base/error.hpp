#pragma once

/// 错误处理模块 / Error handling module
///
/// 合并 Rust error.rs 为单文件 1:1 对应。
/// Merged from code.hpp/error.hpp/ex_error.hpp/lazy_error.hpp to match Rust error.rs 1:1.
///
/// 内容：
///   - ErrorCode 错误码枚举
///   - ErrorCodeTrait concept
///   - Error<C> 错误基类
///   - ExError<C, V> 带关联值的错误
///   - LazyError<C> 延迟消息构造的错误
///   - error_code_from_u32 安全转换

#include <cstdint>
#include <functional>
#include <string>
#include <type_traits>
#include <utility>

namespace ospf::base {

    // ============================================================================
    // ErrorCode 错误码枚举 / Error code enumeration
    // ============================================================================

    enum class ErrorCode : std::uint32_t {
        Ok                  = 0x00,
        AuthenticationError = 0x01,

        // 文件/数据 / File & data
        NotAFile                = 0x10,
        NotADirectory           = 0x11,
        FileNotFound            = 0x12,
        DirectoryUnusable       = 0x13,
        FileExtensionNotMatched = 0x14,
        DataNotFound            = 0x15,
        DataEmpty               = 0x16,
        SerializationFailed     = 0x1a,
        DeserializationFailed   = 0x1b,

        // 建模 / Modeling
        TokenExisted     = 0x20,
        SymbolRepetitive = 0x21,
        LackOfPipelines  = 0x22,
        SolverNotFound   = 0x23,

        // OR 引擎 / OR engine
        OREngineEnvironmentLost      = 0x24,
        OREngineConnectionOvertime   = 0x25,
        OREngineModelingException    = 0x26,
        OREngineSolvingException     = 0x27,
        OREngineTerminated           = 0x28,
        ORModelInfeasible            = 0x29,
        ORModelUnbounded             = 0x2a,
        ORModelInfeasibleOrUnbounded = 0x2b,
        ORSolutionInvalid            = 0x2c,

        // 应用 / Application
        ApplicationFailed    = 0x30,
        ApplicationError     = 0x31,
        ApplicationException = 0x32,
        ApplicationStopped   = 0x33,
        IllegalArgument      = 0x34,
        IllegalState         = 0x35,
        NotImplemented       = 0x36,

        // 特殊 / Special
        Other   = 0xfe,
        Unknown = 0xff,
    };

    /// 从底层整数安全转换，未知值返回 Unknown。
    /// Safely convert from underlying integer; unknown values return Unknown.
    [[nodiscard]] constexpr ErrorCode error_code_from_u32(std::uint32_t value) noexcept {
        switch (value) {
            case 0x00: return ErrorCode::Ok;
            case 0x01: return ErrorCode::AuthenticationError;
            case 0x10: return ErrorCode::NotAFile;
            case 0x11: return ErrorCode::NotADirectory;
            case 0x12: return ErrorCode::FileNotFound;
            case 0x13: return ErrorCode::DirectoryUnusable;
            case 0x14: return ErrorCode::FileExtensionNotMatched;
            case 0x15: return ErrorCode::DataNotFound;
            case 0x16: return ErrorCode::DataEmpty;
            case 0x1a: return ErrorCode::SerializationFailed;
            case 0x1b: return ErrorCode::DeserializationFailed;
            case 0x20: return ErrorCode::TokenExisted;
            case 0x21: return ErrorCode::SymbolRepetitive;
            case 0x22: return ErrorCode::LackOfPipelines;
            case 0x23: return ErrorCode::SolverNotFound;
            case 0x24: return ErrorCode::OREngineEnvironmentLost;
            case 0x25: return ErrorCode::OREngineConnectionOvertime;
            case 0x26: return ErrorCode::OREngineModelingException;
            case 0x27: return ErrorCode::OREngineSolvingException;
            case 0x28: return ErrorCode::OREngineTerminated;
            case 0x29: return ErrorCode::ORModelInfeasible;
            case 0x2a: return ErrorCode::ORModelUnbounded;
            case 0x2b: return ErrorCode::ORModelInfeasibleOrUnbounded;
            case 0x2c: return ErrorCode::ORSolutionInvalid;
            case 0x30: return ErrorCode::ApplicationFailed;
            case 0x31: return ErrorCode::ApplicationError;
            case 0x32: return ErrorCode::ApplicationException;
            case 0x33: return ErrorCode::ApplicationStopped;
            case 0x34: return ErrorCode::IllegalArgument;
            case 0x35: return ErrorCode::IllegalState;
            case 0x36: return ErrorCode::NotImplemented;
            case 0xfe: return ErrorCode::Other;
            case 0xff: return ErrorCode::Unknown;
            default:   return ErrorCode::Unknown;
        }
    }

    // ============================================================================
    // ErrorCodeTrait concept
    // ============================================================================

    /// 错误码 trait / Error code trait
    /// 约束错误码类型必须是枚举且底层可转为 uint32_t。
    /// Constrains error code types to be enums with uint32_t-compatible underlying type.
    template<typename C>
    concept ErrorCodeTrait = std::is_enum_v<C>
        && requires(C code) {
            { static_cast<std::underlying_type_t<C>>(code) } -> std::convertible_to<std::uint32_t>;
        };

    // ============================================================================
    // Error<C> 错误基类 / Error base class
    // ============================================================================

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

    // ============================================================================
    // ExError<C, V> 带关联值的错误 / Error with associated value
    // ============================================================================

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

    // ============================================================================
    // LazyError<C> 延迟消息构造的错误 / Lazy error with deferred message construction
    // ============================================================================

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
