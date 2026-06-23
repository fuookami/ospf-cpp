#pragma once

/// 存储类型标记 / Store type markers
/// 用于区分不同的存储策略。

namespace ospf::base {

    /// 值存储 / Value storage
    struct ValueStore {};

    /// 引用存储 / Reference storage
    struct ReferenceStore {};

    /// 指针存储 / Pointer storage
    struct PointerStore {};

    /// 值或引用存储 / Value-or-reference storage
    struct ValueOrReferenceStore {};

}  // namespace ospf::base
