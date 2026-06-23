#pragma once

/// Optional 扩展 / Optional extensions
/// 提供 Result 与 std::optional 的互操作。

#include <optional>
#include <string>

namespace ospf::base {

    /// 从 optional 和错误信息构造 Result / Construct Result from optional and error message
    template<typename T>
    [[nodiscard]] auto from_optional(std::optional<T> opt, std::string error_msg)
        -> std::optional<T>;

}  // namespace ospf::base
