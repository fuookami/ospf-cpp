#pragma once

/// 索引类型模块 / Indexed type module
/// 提供自动生成和手动设置的索引类型，以及索引生成器。

#include <atomic>
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <unordered_map>

namespace ospf::base {

    // ---------------------------------------------------------------------------
    // IndexGenerator — 全局索引生成器 / Global index generator
    // ---------------------------------------------------------------------------
    class IndexGenerator {
    public:
        [[nodiscard]] static IndexGenerator& instance() {
            static IndexGenerator gen;
            return gen;
        }

        [[nodiscard]] std::size_t next() {
            std::lock_guard lock(mutex_);
            return counter_++;
        }

        void flush() {
            std::lock_guard lock(mutex_);
            counter_ = 0;
        }

    private:
        IndexGenerator() = default;
        std::mutex mutex_;
        std::size_t counter_ = 0;
    };

    /// 基于类型的索引生成器 / Type-based index generator
    template<typename Tag>
    class TypedIndexGenerator {
    public:
        [[nodiscard]] static TypedIndexGenerator& instance() {
            static TypedIndexGenerator gen;
            return gen;
        }

        [[nodiscard]] std::size_t next() {
            std::lock_guard lock(mutex_);
            return counter_++;
        }

        void flush() {
            std::lock_guard lock(mutex_);
            counter_ = 0;
        }

    private:
        TypedIndexGenerator() = default;
        std::mutex mutex_;
        std::size_t counter_ = 0;
    };

    // ---------------------------------------------------------------------------
    // Index<T> — 自动生成的索引类型 / Automatically generated index type
    // ---------------------------------------------------------------------------
    template<typename Tag>
    class Index {
    public:
        Index() : index_(TypedIndexGenerator<Tag>::instance().next()) {}

        explicit Index(std::size_t index) : index_(index) {}

        [[nodiscard]] std::size_t value() const noexcept { return index_; }
        [[nodiscard]] operator std::size_t() const noexcept { return index_; }

        [[nodiscard]] bool operator==(const Index& other) const noexcept { return index_ == other.index_; }
        [[nodiscard]] bool operator!=(const Index& other) const noexcept { return index_ != other.index_; }
        [[nodiscard]] auto operator<=>(const Index& other) const noexcept { return index_ <=> other.index_; }

        friend std::ostream& operator<<(std::ostream& os, const Index& idx) {
            return os << idx.index_;
        }

    private:
        std::size_t index_;
    };

    // ---------------------------------------------------------------------------
    // ManualIndex<T> — 手动设置的索引类型 / Manually set index type
    // ---------------------------------------------------------------------------
    template<typename Tag>
    class ManualIndex {
    public:
        ManualIndex() = default;

        [[nodiscard]] bool indexed() const noexcept { return index_.has_value(); }

        void set_index(std::size_t index) { index_ = index; }

        [[nodiscard]] std::size_t value() const {
            return index_.value();
        }

        [[nodiscard]] operator std::size_t() const {
            return index_.value();
        }

        [[nodiscard]] bool operator==(const ManualIndex& other) const noexcept {
            return index_ == other.index_;
        }

        [[nodiscard]] bool operator!=(const ManualIndex& other) const noexcept {
            return index_ != other.index_;
        }

        friend std::ostream& operator<<(std::ostream& os, const ManualIndex& idx) {
            if (idx.indexed()) {
                return os << *idx.index_;
            }
            return os << "<unindexed>";
        }

    private:
        std::optional<std::size_t> index_;
    };

    // ---------------------------------------------------------------------------
    // Indexed trait — 可索引概念 / Indexable concept
    // ---------------------------------------------------------------------------
    template<typename T>
    concept Indexable = requires(const T& t) {
        { t.index() } -> std::convertible_to<std::size_t>;
    };

    // ---------------------------------------------------------------------------
    // IndexedSliceExt — 可索引切片查找扩展 / Indexed slice lookup extension
    // ---------------------------------------------------------------------------
    template<Indexable T>
    [[nodiscard]] const T* find_or_get(const std::vector<T>& items, std::size_t index) {
        // 先按 index 查找 / Find by index first
        for (const auto& item : items) {
            if (item.index() == index) {
                return &item;
            }
        }
        // 再按位置获取 / Then by position
        if (index < items.size()) {
            return &items[index];
        }
        return nullptr;
    }

}  // namespace ospf::base
