#pragma once

/// 值区间 / Value range
/// 支持开闭区间、无穷边界、区间运算。

#include <algorithm>
#include <cmath>
#include <limits>
#include <optional>
#include <ostream>

namespace ospf::math::algebra {

    // -- 区间类型标记 / Interval type markers ----------------------------------

    /// 闭区间标记 / Closed interval marker
    struct Closed {};

    /// 开区间标记 / Open interval marker
    struct Open {};

    // -- 边界值 / Bound value -------------------------------------------------

    /// 边界值：支持有限值、正无穷、负无穷 / Bound value: finite, +inf, -inf
    template<typename T>
    class ValueWrapper {
    public:
        [[nodiscard]] static ValueWrapper finite(T value) {
            return ValueWrapper(std::move(value), false, false);
        }

        [[nodiscard]] static ValueWrapper positive_infinity() {
            return ValueWrapper(T{}, true, false);
        }

        [[nodiscard]] static ValueWrapper negative_infinity() {
            return ValueWrapper(T{}, false, true);
        }

        [[nodiscard]] bool is_finite()           const noexcept { return !pos_inf_ && !neg_inf_; }
        [[nodiscard]] bool is_positive_infinity() const noexcept { return pos_inf_; }
        [[nodiscard]] bool is_negative_infinity() const noexcept { return neg_inf_; }

        [[nodiscard]] const T& value() const { return value_; }

        [[nodiscard]] bool operator==(const ValueWrapper& other) const noexcept {
            if (pos_inf_ != other.pos_inf_ || neg_inf_ != other.neg_inf_) return false;
            if (pos_inf_ || neg_inf_) return true;
            return value_ == other.value_;
        }

        [[nodiscard]] bool operator<(const ValueWrapper& other) const noexcept {
            if (neg_inf_) return !other.neg_inf_;
            if (pos_inf_) return false;
            if (other.neg_inf_) return false;
            if (other.pos_inf_) return true;
            return value_ < other.value_;
        }

        [[nodiscard]] bool operator>(const ValueWrapper& other) const noexcept { return other < *this; }
        [[nodiscard]] bool operator<=(const ValueWrapper& other) const noexcept { return !(other < *this); }
        [[nodiscard]] bool operator>=(const ValueWrapper& other) const noexcept { return !(*this < other); }

        friend std::ostream& operator<<(std::ostream& os, const ValueWrapper& v) {
            if (v.pos_inf_) return os << "+inf";
            if (v.neg_inf_) return os << "-inf";
            return os << v.value_;
        }

    private:
        ValueWrapper(T value, bool pos_inf, bool neg_inf)
            : value_(std::move(value)), pos_inf_(pos_inf), neg_inf_(neg_inf) {}

        T value_;
        bool pos_inf_;
        bool neg_inf_;
    };

    // -- 边界 / Bound --------------------------------------------------------

    /// 区间边界 / Interval bound
    template<typename T, typename IntervalType = Closed>
    class Bound {
    public:
        Bound(ValueWrapper<T> value, IntervalType interval = {})
            : value_(std::move(value)), interval_(interval) {}

        [[nodiscard]] const ValueWrapper<T>& value() const noexcept { return value_; }
        [[nodiscard]] const IntervalType& interval() const noexcept { return interval_; }

        [[nodiscard]] bool is_closed() const noexcept { return std::is_same_v<IntervalType, Closed>; }
        [[nodiscard]] bool is_open()   const noexcept { return std::is_same_v<IntervalType, Open>; }

    private:
        ValueWrapper<T> value_;
        IntervalType interval_;
    };

    // -- 值区间 / ValueRange -------------------------------------------------

    /// 值区间 / Value range
    template<typename T, typename IL = Closed, typename IU = Closed>
    class ValueRange {
    public:
        ValueRange(Bound<T, IL> lower, Bound<T, IU> upper)
            : lower_(std::move(lower)), upper_(std::move(upper)) {}

        /// 从值构造闭区间 / Construct closed range from values
        [[nodiscard]] static ValueRange<T, Closed, Closed> closed(T lower, T upper) {
            return {
                Bound<T, Closed>(ValueWrapper<T>::finite(std::move(lower)), Closed{}),
                Bound<T, Closed>(ValueWrapper<T>::finite(std::move(upper)), Closed{})
            };
        }

        /// 从值构造开区间 / Construct open range from values
        [[nodiscard]] static ValueRange<T, Open, Open> open(T lower, T upper) {
            return {
                Bound<T, Open>(ValueWrapper<T>::finite(std::move(lower)), Open{}),
                Bound<T, Open>(ValueWrapper<T>::finite(std::move(upper)), Open{})
            };
        }

        /// 从值构造左闭右开区间 / Construct left-closed right-open range
        [[nodiscard]] static ValueRange<T, Closed, Open> left_closed_right_open(T lower, T upper) {
            return {
                Bound<T, Closed>(ValueWrapper<T>::finite(std::move(lower)), Closed{}),
                Bound<T, Open>(ValueWrapper<T>::finite(std::move(upper)), Open{})
            };
        }

        [[nodiscard]] const Bound<T, IL>& lower_bound() const noexcept { return lower_; }
        [[nodiscard]] const Bound<T, IU>& upper_bound() const noexcept { return upper_; }

        /// 检查值是否在区间内 / Check if value is in range
        [[nodiscard]] bool contains(const T& value) const {
            bool above_lower = lower_.value().is_negative_infinity() ||
                (lower_.value().is_finite() && (
                    lower_.is_closed() ? value >= lower_.value().value() : value > lower_.value().value()));
            bool below_upper = upper_.value().is_positive_infinity() ||
                (upper_.value().is_finite() && (
                    upper_.is_closed() ? value <= upper_.value().value() : value < upper_.value().value()));
            return above_lower && below_upper;
        }

        /// 获取区间长度（仅有限区间）/ Get range length (finite ranges only)
        [[nodiscard]] std::optional<T> length() const {
            if (lower_.value().is_finite() && upper_.value().is_finite()) {
                return upper_.value().value() - lower_.value().value();
            }
            return std::nullopt;
        }

        /// 检查区间是否为空 / Check if range is empty
        [[nodiscard]] bool is_empty() const {
            if (!lower_.value().is_finite() || !upper_.value().is_finite()) return false;
            auto lo = lower_.value().value();
            auto hi = upper_.value().value();
            if (lo > hi) return true;
            if (lo == hi && (lower_.is_open() || upper_.is_open())) return true;
            return false;
        }

    private:
        Bound<T, IL> lower_;
        Bound<T, IU> upper_;
    };

    // -- 便捷类型别名 / Convenience aliases -----------------------------------

    template<typename T>
    using ClosedRange = ValueRange<T, Closed, Closed>;

    template<typename T>
    using OpenRange = ValueRange<T, Open, Open>;

    template<typename T>
    using LeftClosedRightOpenRange = ValueRange<T, Closed, Open>;

}  // namespace ospf::math::algebra
