#pragma once

/// 错误码枚举 / Error code enumeration
/// 基础层使用 0x00–0x3F，各领域模块使用 0x1000+ 按区块分配。
/// Base layer uses 0x00-0x3F; domain modules use 0x1000+ per block allocation.

#include <cstdint>
#include <type_traits>

namespace ospf::base {

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
        TokenExisted    = 0x20,
        SymbolRepetitive = 0x21,
        LackOfPipelines = 0x22,
        SolverNotFound  = 0x23,

        // OR 引擎 / OR engine
        OREngineEnvironmentLost     = 0x24,
        OREngineConnectionOvertime  = 0x25,
        OREngineModelingException   = 0x26,
        OREngineSolvingException    = 0x27,
        OREngineTerminated          = 0x28,
        ORModelInfeasible           = 0x29,
        ORModelUnbounded            = 0x2a,
        ORModelInfeasibleOrUnbounded = 0x2b,
        ORSolutionInvalid           = 0x2c,

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
        // Accept known values; anything else → Unknown
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

    /// ErrorCodeTrait: 约束错误码类型必须是枚举且底层可转为 uint32_t。
    /// ErrorCodeTrait: constrains error code types to be enums with uint32_t-compatible underlying type.
    template<typename C>
    concept ErrorCodeTrait = std::is_enum_v<C>
        && requires(C code) {
            { static_cast<std::underlying_type_t<C>>(code) } -> std::convertible_to<std::uint32_t>;
        };

}  // namespace ospf::base
