#pragma once

/// 元编程工具 / Meta-programming utilities
/// 提供类型特征和编译期计算。

#include <cstddef>
#include <type_traits>

namespace ospf::base {

    // -- 类型列表 / Type list --------------------------------------------------

    template<typename... Ts>
    struct TypeList {};

    /// 类型列表大小 / Type list size
    template<typename List>
    struct TypeListSize;

    template<typename... Ts>
    struct TypeListSize<TypeList<Ts...>> {
        static constexpr std::size_t value = sizeof...(Ts);
    };

    template<typename List>
    inline constexpr std::size_t TypeListSize_v = TypeListSize<List>::value;

    // -- 类型特征 / Type traits ------------------------------------------------

    /// 是否为实例化模板 / Is template instantiation
    template<typename T, template<typename...> class Template>
    struct is_instantiation : std::false_type {};

    template<template<typename...> class Template, typename... Args>
    struct is_instantiation<Template<Args...>, Template> : std::true_type {};

    template<typename T, template<typename...> class Template>
    inline constexpr bool is_instantiation_v = is_instantiation<T, Template>::value;

    // -- 常量值 / Constant values ----------------------------------------------

    template<typename T, T Value>
    struct Constant {
        static constexpr T value = Value;
        using type = Constant;
        constexpr operator T() const noexcept { return value; }
    };

    template<std::size_t N>
    using SizeT = Constant<std::size_t, N>;

    // -- 条件类型 / Conditional type -------------------------------------------

    template<bool Condition, typename TrueType, typename FalseType>
    using Conditional = std::conditional_t<Condition, TrueType, FalseType>;

}  // namespace ospf::base
