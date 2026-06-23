#pragma once

/// 智能指针工具 / Smart pointer utilities

#include <memory>
#include <utility>

namespace ospf::base {

    /// 安全解引用 unique_ptr / Safe deref unique_ptr
    template<typename T>
    [[nodiscard]] T& deref(const std::unique_ptr<T>& ptr) {
        return *ptr;
    }

    /// 安全解引用 shared_ptr / Safe deref shared_ptr
    template<typename T>
    [[nodiscard]] T& deref(const std::shared_ptr<T>& ptr) {
        return *ptr;
    }

    /// 转移所有权或创建新指针 / Transfer ownership or create new
    template<typename T, typename... Args>
    [[nodiscard]] std::unique_ptr<T> make_unique_or_move(
        std::unique_ptr<T> existing, Args&&... args)
    {
        if (existing) return std::move(existing);
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

}  // namespace ospf::base
