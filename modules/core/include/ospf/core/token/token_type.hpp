#pragma once
/// Token type /// 1:1 对应 Rust core/token/token_type.rs
/// Token 类型枚举

#include <cstdint>

namespace ospf::core {

    /// Token 类型 / Token type
    enum class TokenType : std::uint8_t {
        Variable,       ///< 变量 token / Variable token
        Constraint,     ///< 约束 token / Constraint token
        Objective,      ///< 目标 token / Objective token
        Intermediate,   ///< 中间符号 token / Intermediate symbol token
    };

}  // namespace ospf::core
