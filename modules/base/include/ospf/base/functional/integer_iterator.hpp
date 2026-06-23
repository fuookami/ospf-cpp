#pragma once

/// 整数迭代器 / Integer iterator
/// 类似 Python range() 或 Rust (0..n)。

#include <compare>
#include <iterator>

namespace ospf::base {

    /// 整数迭代器 / Integer iterator
    template<typename T = int>
    class IntegerIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const T*;
        using reference         = const T&;

        constexpr explicit IntegerIterator(T current, T step = T{1})
            : current_(current), step_(step) {}

        [[nodiscard]] constexpr const T& operator*()  const noexcept { return current_; }
        [[nodiscard]] constexpr const T* operator->() const noexcept { return &current_; }

        constexpr IntegerIterator& operator++() noexcept {
            current_ += step_;
            return *this;
        }

        constexpr IntegerIterator operator++(int) noexcept {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        [[nodiscard]] constexpr bool operator==(const IntegerIterator& other) const noexcept {
            return current_ == other.current_;
        }

        [[nodiscard]] constexpr std::strong_ordering operator<=>(const IntegerIterator& other) const noexcept {
            return current_ <=> other.current_;
        }

    private:
        T current_;
        T step_;
    };

    /// 整数范围 / Integer range
    template<typename T = int>
    class IntegerRange {
    public:
        constexpr IntegerRange(T start, T end, T step = T{1})
            : start_(start), end_(end), step_(step) {}

        [[nodiscard]] constexpr IntegerIterator<T> begin() const noexcept {
            return IntegerIterator<T>(start_, step_);
        }

        [[nodiscard]] constexpr IntegerIterator<T> end() const noexcept {
            return IntegerIterator<T>(end_, step_);
        }

        [[nodiscard]] constexpr std::size_t size() const noexcept {
            if (step_ <= T{0} || start_ >= end_) return 0;
            return static_cast<std::size_t>((end_ - start_ + step_ - T{1}) / step_);
        }

    private:
        T start_;
        T end_;
        T step_;
    };

    /// 便捷构造 / Convenience factory
    template<typename T>
    [[nodiscard]] constexpr auto range(T end) noexcept {
        return IntegerRange<T>(T{0}, end);
    }

    template<typename T>
    [[nodiscard]] constexpr auto range(T start, T end) noexcept {
        return IntegerRange<T>(start, end);
    }

    template<typename T>
    [[nodiscard]] constexpr auto range(T start, T end, T step) noexcept {
        return IntegerRange<T>(start, end, step);
    }

}  // namespace ospf::base
