#pragma once

/// 代数结构概念 / Algebraic structure concepts
/// 半群、幺半群、群、阿贝尔群、环、域等。

#include <ospf/math/algebra/real_number.hpp>
#include <concepts>
#include <type_traits>

namespace ospf::math::algebra {

    // -- 半群 / Semigroup ----------------------------------------------------

    /// 半群：封闭 + 结合律 / Semigroup: closure + associativity
    template<typename T>
    concept Semigroup = requires(const T& a, const T& b) {
        { a + b } -> std::convertible_to<T>;
    };

    // -- 幺半群 / Monoid -----------------------------------------------------

    /// 幺半群：半群 + 单位元 / Monoid: semigroup + identity
    /// 对于算术类型，零值通过 T{} 获得
    template<typename T>
    concept Monoid = Semigroup<T> && requires {
        { T{} } -> std::convertible_to<T>;
    };

    // -- 群 / Group -----------------------------------------------------------

    /// 群：幺半群 + 逆元 / Group: monoid + inverse
    template<typename T>
    concept Group = Monoid<T> && requires(const T& a) {
        { -a } -> std::convertible_to<T>;
        { a - a } -> std::convertible_to<T>;
    };

    // -- 阿贝尔群 / AbelianGroup ----------------------------------------------

    /// 阿贝尔群（交换群）：群 + 交换律 / Abelian group: group + commutativity
    template<typename T>
    concept AbelianGroup = Group<T>;

    // -- 乘法半群 / MultiplicativeSemigroup -----------------------------------

    /// 乘法半群 / Multiplicative semigroup
    template<typename T>
    concept MultiplicativeSemigroup = requires(const T& a, const T& b) {
        { a * b } -> std::convertible_to<T>;
    };

    // -- 乘法幺半群 / MultiplicativeMonoid ------------------------------------

    /// 乘法幺半群 / Multiplicative monoid
    /// 对于算术类型，单位元通过 T{1} 获得
    template<typename T>
    concept MultiplicativeMonoid = MultiplicativeSemigroup<T> && requires {
        { T{1} } -> std::convertible_to<T>;
    };

    // -- 乘法群 / MultiplicativeGroup ----------------------------------------

    /// 乘法群 / Multiplicative group
    template<typename T>
    concept MultiplicativeGroup = MultiplicativeMonoid<T> && requires(const T& a) {
        { a / a } -> std::convertible_to<T>;
    };

    // -- 环 / Ring ------------------------------------------------------------

    /// 环：阿贝尔群 + 乘法幺半群 / Ring: abelian group + multiplicative monoid
    template<typename T>
    concept Ring = AbelianGroup<T> && MultiplicativeMonoid<T>;

    // -- 交换环 / CommutativeRing ---------------------------------------------

    /// 交换环 / Commutative ring
    template<typename T>
    concept CommutativeRing = Ring<T>;

    // -- 域 / Field -----------------------------------------------------------

    /// 域：交换环 + 乘法群 + 实数约束 / Field: commutative ring + multiplicative group + real number
    /// Field refine RealNumber，确保数值语义完整。
    /// Field refines RealNumber, ensuring complete numeric semantics.
    template<typename T>
    concept Field = CommutativeRing<T> && MultiplicativeGroup<T> && ospf::math::RealNumber<T>;

    // -- 全序 / TotallyOrdered ------------------------------------------------

    /// 全序类型 / Totally ordered type
    template<typename T>
    concept TotallyOrdered = requires(const T& a, const T& b) {
        { a < b } -> std::convertible_to<bool>;
        { a <= b } -> std::convertible_to<bool>;
        { a > b } -> std::convertible_to<bool>;
        { a >= b } -> std::convertible_to<bool>;
    };

    // -- 有界 / Bounded -------------------------------------------------------

    /// 有界类型 / Bounded type
    template<typename T>
    concept Bounded = requires {
        { T::min_value() } -> std::convertible_to<T>;
        { T::max_value() } -> std::convertible_to<T>;
    };

    // -- 固定点 / Fixed -------------------------------------------------------

    /// 固定点类型 / Fixed point type
    template<typename T>
    concept Fixed = requires {
        { T::epsilon() } -> std::convertible_to<T>;
    };

    // -- 无穷 / Infinite ------------------------------------------------------

    /// 无穷类型 / Infinite type
    template<typename T>
    concept Infinite = requires {
        { T::positive_infinity() } -> std::convertible_to<T>;
        { T::negative_infinity() } -> std::convertible_to<T>;
    };

    // -- 标量 / Scalar --------------------------------------------------------

    /// 标量类型：算术 + 全序 / Scalar type: arithmetic + totally ordered
    /// 对于算术类型，直接检查运算符和全序
    template<typename T>
    concept Scalar = AbelianGroup<T> && MultiplicativeMonoid<T>
        && TotallyOrdered<T>
        && requires(const T& a, const T& b) {
            { a * b } -> std::convertible_to<T>;
            { a / b } -> std::convertible_to<T>;
        };

    // -- 向量空间 / VectorSpace -----------------------------------------------

    /// 向量空间 / Vector space
    template<typename V, typename S = double>
    concept VectorSpace = requires(const V& a, const V& b, const S& s) {
        { a + b } -> std::convertible_to<V>;
        { a - b } -> std::convertible_to<V>;
        { s * a } -> std::convertible_to<V>;
        { V::zero() } -> std::convertible_to<V>;
    };

    // -- 内积空间 / InnerProductSpace -----------------------------------------

    /// 内积空间 / Inner product space
    template<typename V, typename S = double>
    concept InnerProductSpace = VectorSpace<V, S> && requires(const V& a, const V& b) {
        { a.dot(b) } -> std::convertible_to<S>;
    };

    // -- 赋范空间 / NormedSpace -----------------------------------------------

    /// 赋范空间 / Normed space
    template<typename V, typename S = double>
    concept NormedSpace = VectorSpace<V, S> && requires(const V& a) {
        { a.norm() } -> std::convertible_to<S>;
    };

    // -- 标量乘法 / Scalar multiplication -------------------------------------

    /// 标量乘法 / Scalar multiplication
    template<typename T>
    concept Scalable = requires(const T& a, double s) {
        { a * s } -> std::convertible_to<T>;
        { s * a } -> std::convertible_to<T>;
    };

}  // namespace ospf::math::algebra
