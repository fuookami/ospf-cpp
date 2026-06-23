#pragma once

/// copy-faster 概念：当移动不比复制快时，用复制 / Copy-faster concept

#include <type_traits>

namespace ospf::base {

    /// 对于 trivially copyable 类型，复制可能不比移动慢。
    /// For trivially copyable types, copy may not be slower than move.
    template<typename T>
    concept CopyFaster = std::is_trivially_copyable_v<T>;

}  // namespace ospf::base
