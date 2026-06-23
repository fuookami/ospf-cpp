#pragma once

/// 枚举概念 / Enum concepts

#include <type_traits>

namespace ospf::base {

    /// 枚举类型 / Enum type
    template<typename T>
    concept EnumType = std::is_enum_v<T>;

    /// 底层值可转为指定类型 / Underlying value convertible to target type
    template<typename T, typename U = std::underlying_type_t<T>>
    concept EnumWithUnderlying = EnumType<T> && std::convertible_to<U, std::underlying_type_t<T>>;

}  // namespace ospf::base
