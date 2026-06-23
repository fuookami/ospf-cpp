#pragma once

/// 带默认值概念 / With-default concept

namespace ospf::base {

    /// 类型具有静态 default() 方法 / Type has static default() method
    template<typename T>
    concept WithDefault = requires {
        { T::default_value() } -> std::same_as<T>;
    };

}  // namespace ospf::base
