#pragma once

/// ChunkedVec — 分块存储容器 / Chunked storage container
/// 将元素存储在固定大小的块中，适合并行处理和大型数组的高效内存管理。
/// Stores elements in fixed-size chunks, suitable for parallel processing and efficient memory management.

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <vector>

namespace ospf::base {

    /// 默认块大小（元素数量） / Default chunk size in elements
    inline constexpr std::size_t DEFAULT_CHUNK_SIZE = 4096;

    /// 分块存储容器 / Chunked storage container
    template<typename T>
    class ChunkedVec {
    public:
        // -- 构造 / Construction ------------------------------------------------

        ChunkedVec() : ChunkedVec(DEFAULT_CHUNK_SIZE) {}

        explicit ChunkedVec(std::size_t chunk_size)
            : chunk_size_(chunk_size) {
            assert(chunk_size > 0);
        }

        ChunkedVec(std::size_t capacity, std::size_t chunk_size)
            : chunk_size_(chunk_size) {
            assert(chunk_size > 0);
            reserve(capacity);
        }

        ChunkedVec(const ChunkedVec&) = default;
        ChunkedVec(ChunkedVec&&) noexcept = default;
        ChunkedVec& operator=(const ChunkedVec&) = default;
        ChunkedVec& operator=(ChunkedVec&&) noexcept = default;
        ~ChunkedVec() = default;

        // -- 容量 / Capacity ---------------------------------------------------

        [[nodiscard]] std::size_t chunk_size()  const noexcept { return chunk_size_; }
        [[nodiscard]] std::size_t chunk_count() const noexcept { return chunks_.size(); }
        [[nodiscard]] std::size_t size()        const noexcept { return len_; }
        [[nodiscard]] bool        empty()       const noexcept { return len_ == 0; }
        [[nodiscard]] std::size_t capacity()    const noexcept { return chunks_.size() * chunk_size_; }

        // -- 元素访问 / Element access ------------------------------------------

        /// 按索引获取（带边界检查） / Get by index (bounds-checked)
        [[nodiscard]] T* get(std::size_t index) {
            if (index >= len_) return nullptr;
            auto ci = index / chunk_size_;
            auto ei = index % chunk_size_;
            return &chunks_[ci][ei];
        }

        [[nodiscard]] const T* get(std::size_t index) const {
            if (index >= len_) return nullptr;
            auto ci = index / chunk_size_;
            auto ei = index % chunk_size_;
            return &chunks_[ci][ei];
        }

        /// operator[]（不检查边界） / operator[] (unchecked)
        [[nodiscard]] T& operator[](std::size_t index) {
            auto ci = index / chunk_size_;
            auto ei = index % chunk_size_;
            return chunks_[ci][ei];
        }

        [[nodiscard]] const T& operator[](std::size_t index) const {
            auto ci = index / chunk_size_;
            auto ei = index % chunk_size_;
            return chunks_[ci][ei];
        }

        // -- 块访问 / Chunk access ---------------------------------------------

        [[nodiscard]] std::vector<T>* chunk(std::size_t chunk_index) {
            if (chunk_index >= chunks_.size()) return nullptr;
            return &chunks_[chunk_index];
        }

        [[nodiscard]] const std::vector<T>* chunk(std::size_t chunk_index) const {
            if (chunk_index >= chunks_.size()) return nullptr;
            return &chunks_[chunk_index];
        }

        [[nodiscard]] std::vector<std::vector<T>>& chunks() { return chunks_; }
        [[nodiscard]] const std::vector<std::vector<T>>& chunks() const { return chunks_; }

        // -- 修改 / Modification -----------------------------------------------

        void push(const T& value) {
            auto ci = len_ / chunk_size_;
            if (ci >= chunks_.size()) {
                chunks_.emplace_back();
                chunks_.back().reserve(chunk_size_);
            }
            chunks_[ci].push_back(value);
            ++len_;
        }

        void push(T&& value) {
            auto ci = len_ / chunk_size_;
            if (ci >= chunks_.size()) {
                chunks_.emplace_back();
                chunks_.back().reserve(chunk_size_);
            }
            chunks_[ci].push_back(std::move(value));
            ++len_;
        }

        template<typename... Args>
        T& emplace(Args&&... args) {
            auto ci = len_ / chunk_size_;
            if (ci >= chunks_.size()) {
                chunks_.emplace_back();
                chunks_.back().reserve(chunk_size_);
            }
            chunks_[ci].emplace_back(std::forward<Args>(args)...);
            ++len_;
            return chunks_[ci].back();
        }

        [[nodiscard]] T pop() {
            assert(len_ > 0);
            --len_;
            auto ci = len_ / chunk_size_;
            T val = std::move(chunks_[ci].back());
            chunks_[ci].pop_back();
            if (chunks_[ci].empty() && ci > 0) {
                chunks_.pop_back();
            }
            return val;
        }

        void reserve(std::size_t additional) {
            auto new_len = len_ + additional;
            auto needed = (new_len + chunk_size_ - 1) / chunk_size_;
            if (needed > chunks_.size()) {
                chunks_.reserve(needed);
                while (chunks_.size() < needed) {
                    chunks_.emplace_back();
                    chunks_.back().reserve(chunk_size_);
                }
            }
        }

        void resize(std::size_t new_len, const T& value) {
            if (new_len > len_) {
                reserve(new_len - len_);
                while (len_ < new_len) {
                    push(value);
                }
            } else {
                while (len_ > new_len) {
                    (void)pop();
                }
            }
        }

        void clear() {
            chunks_.clear();
            len_ = 0;
        }

        // -- 转换 / Conversion -------------------------------------------------

        [[nodiscard]] std::vector<T> to_vec() const {
            std::vector<T> result;
            result.reserve(len_);
            for (const auto& chunk : chunks_) {
                result.insert(result.end(), chunk.begin(), chunk.end());
            }
            return result;
        }

        // -- 迭代器 / Iterators -------------------------------------------------

        class iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type        = T;
            using difference_type   = std::ptrdiff_t;
            using pointer           = T*;
            using reference         = T&;

            iterator(ChunkedVec* vec, std::size_t index)
                : vec_(vec), index_(index) {}

            [[nodiscard]] T& operator*()  const { return (*vec_)[index_]; }
            [[nodiscard]] T* operator->() const { return &(*vec_)[index_]; }

            iterator& operator++() { ++index_; return *this; }
            iterator operator++(int) { auto tmp = *this; ++index_; return tmp; }

            [[nodiscard]] bool operator==(const iterator& other) const { return index_ == other.index_; }
            [[nodiscard]] bool operator!=(const iterator& other) const { return index_ != other.index_; }

        private:
            ChunkedVec* vec_;
            std::size_t index_;
        };

        class const_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type        = T;
            using difference_type   = std::ptrdiff_t;
            using pointer           = const T*;
            using reference         = const T&;

            const_iterator(const ChunkedVec* vec, std::size_t index)
                : vec_(vec), index_(index) {}

            [[nodiscard]] const T& operator*()  const { return (*vec_)[index_]; }
            [[nodiscard]] const T* operator->() const { return &(*vec_)[index_]; }

            const_iterator& operator++() { ++index_; return *this; }
            const_iterator operator++(int) { auto tmp = *this; ++index_; return tmp; }

            [[nodiscard]] bool operator==(const const_iterator& other) const { return index_ == other.index_; }
            [[nodiscard]] bool operator!=(const const_iterator& other) const { return index_ != other.index_; }

        private:
            const ChunkedVec* vec_;
            std::size_t index_;
        };

        [[nodiscard]] iterator begin() { return iterator(this, 0); }
        [[nodiscard]] iterator end()   { return iterator(this, len_); }
        [[nodiscard]] const_iterator begin()  const { return const_iterator(this, 0); }
        [[nodiscard]] const_iterator end()    const { return const_iterator(this, len_); }
        [[nodiscard]] const_iterator cbegin() const { return const_iterator(this, 0); }
        [[nodiscard]] const_iterator cend()   const { return const_iterator(this, len_); }

    private:
        std::vector<std::vector<T>> chunks_;
        std::size_t chunk_size_ = DEFAULT_CHUNK_SIZE;
        std::size_t len_ = 0;
    };

}  // namespace ospf::base
