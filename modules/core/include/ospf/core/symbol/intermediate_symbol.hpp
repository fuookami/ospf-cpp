#pragma once
/// IntermediateSymbol trait 体系 / IntermediateSymbol trait system
/// 1:1 对应 Rust core/symbol/intermediate_symbol.rs
///
/// 连接 function evaluate 与 MetaModel token/约束生成的桥梁。
/// Bridge connecting function evaluate to MetaModel token/constraint generation.

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <optional>
#include <functional>
#include <atomic>

namespace ospf::core {

    // ============================================================================
    // IntermediateSymbolId — 中间符号唯一标识 / Intermediate symbol unique identifier
    // ============================================================================

    /// 中间符号唯一标识 / Intermediate symbol unique identifier
    struct IntermediateSymbolId {
        std::uint64_t id;
        std::string name;

        [[nodiscard]] static IntermediateSymbolId from_id(std::uint64_t id) {
            return {id, "sym_" + std::to_string(id)};
        }

        [[nodiscard]] bool operator==(const IntermediateSymbolId& other) const noexcept {
            return id == other.id;
        }
    };

    /// 自动 ID 生成器 / Auto ID generator
    inline std::atomic<std::uint64_t> next_auto_intermediate_symbol_id_{1'000'000'000};

    [[nodiscard]] inline std::uint64_t next_auto_intermediate_symbol_id() {
        return next_auto_intermediate_symbol_id_.fetch_add(1, std::memory_order_relaxed);
    }

    [[nodiscard]] inline std::string auto_intermediate_symbol_name(
        const std::string& prefix, std::uint64_t id)
    {
        return prefix + "_" + std::to_string(id);
    }

    // ============================================================================
    // Category — 符号类别 / Symbol category
    // ============================================================================

    /// 符号类别 / Symbol category
    enum class Category : std::uint8_t {
        Linear = 0,
        Quadratic = 1,
        Polynomial = 2,
        Nonlinear = 3,
    };

    // ============================================================================
    // VariableRange — 变量范围 / Variable range
    // ============================================================================

    /// 变量范围 / Variable range
    template<typename V = double>
    struct VariableRange {
        V lower;
        V upper;
    };

    // ============================================================================
    // LinearConstraint / QuadraticConstraint — 约束类型 / Constraint types
    // ============================================================================

    /// 线性约束 / Linear constraint
    template<typename V = double>
    struct LinearConstraint {
        std::vector<std::pair<std::size_t, V>> coefficients;  ///< (变量索引, 系数) / (variable index, coefficient)
        V rhs;                                                 ///< 右端值 / Right-hand side
        enum class Sense : std::uint8_t { LessEqual, GreaterEqual, Equal } sense;
    };

    /// 二次约束 / Quadratic constraint
    template<typename V = double>
    struct QuadraticConstraint {
        struct QuadraticTerm { std::size_t var_i; std::size_t var_j; V coefficient; };
        std::vector<QuadraticTerm> quadratic_coefficients;
        std::vector<std::pair<std::size_t, V>> linear_coefficients;
        V rhs;
        enum class Sense : std::uint8_t { LessEqual, GreaterEqual, Equal } sense;
    };

    // ============================================================================
    // ValueCacheKey / RangeCacheKey — 缓存键 / Cache keys
    // ============================================================================

    /// 值缓存键 / Value cache key
    struct ValueCacheKey {
        std::uint64_t symbol_id;

        [[nodiscard]] static ValueCacheKey from_symbol(std::uint64_t id) {
            return {id};
        }

        [[nodiscard]] bool operator==(const ValueCacheKey& other) const noexcept {
            return symbol_id == other.symbol_id;
        }
    };

    /// 范围缓存键 / Range cache key
    struct RangeCacheKey {
        std::uint64_t symbol_id;

        [[nodiscard]] static RangeCacheKey from_symbol(std::uint64_t id) {
            return {id};
        }

        [[nodiscard]] bool operator==(const RangeCacheKey& other) const noexcept {
            return symbol_id == other.symbol_id;
        }
    };

    // ============================================================================
    // ValueCacheContextTrait / RangeCacheContextTrait — 缓存上下文 / Cache contexts
    // ============================================================================

    /// 值缓存上下文 trait / Value cache context trait
    template<typename V = double>
    class ValueCacheContextTrait {
    public:
        virtual ~ValueCacheContextTrait() = default;

        [[nodiscard]] virtual std::optional<V> get(ValueCacheKey key) const = 0;
        virtual void set(ValueCacheKey key, V value) = 0;
    };

    /// 范围缓存上下文 trait / Range cache context trait
    template<typename V = double>
    class RangeCacheContextTrait {
    public:
        virtual ~RangeCacheContextTrait() = default;

        [[nodiscard]] virtual std::optional<VariableRange<V>> get(RangeCacheKey key) const = 0;
        virtual void set(RangeCacheKey key, VariableRange<V> range) = 0;
    };

    // ============================================================================
    // Token — 符号 token / Symbol token
    // ============================================================================

    /// 符号 token / Symbol token
    template<typename V = double>
    class Token {
    public:
        Token(IntermediateSymbolId id, std::optional<V> value = std::nullopt)
            : id_(std::move(id)), value_(value) {}

        [[nodiscard]] const IntermediateSymbolId& id() const noexcept { return id_; }
        [[nodiscard]] std::optional<V> value() const noexcept { return value_; }
        void set_value(V value) { value_ = value; }

    private:
        IntermediateSymbolId id_;
        std::optional<V> value_;
    };

    /// Token 列表 trait / Token list trait
    template<typename V = double>
    class TokenListTrait {
    public:
        virtual ~TokenListTrait() = default;

        [[nodiscard]] virtual std::optional<V> find_by_id(const IntermediateSymbolId& id) const = 0;
        [[nodiscard]] virtual std::optional<V> find_by_index(std::size_t index) const = 0;
    };

    // ============================================================================
    // IntermediateSymbolEvalContext — 求值上下文 / Evaluation context
    // ============================================================================

    /// 中间符号求值上下文 / Intermediate symbol evaluation context
    template<typename V = double>
    class IntermediateSymbolEvalContext {
    public:
        IntermediateSymbolEvalContext(
            const std::unordered_map<std::size_t, V>& values,
            ValueCacheContextTrait<V>& cache,
            const TokenListTrait<V>* token_list = nullptr
        ) : values_(values), cache_(cache), token_list_(token_list) {}

        [[nodiscard]] const std::unordered_map<std::size_t, V>& values() const noexcept { return values_; }
        [[nodiscard]] ValueCacheContextTrait<V>& cache() noexcept { return cache_; }
        [[nodiscard]] const TokenListTrait<V>* token_list() const noexcept { return token_list_; }

    private:
        const std::unordered_map<std::size_t, V>& values_;
        ValueCacheContextTrait<V>& cache_;
        const TokenListTrait<V>* token_list_;
    };

    // ============================================================================
    // IntermediateSymbol — 核心 trait / Core trait
    // ============================================================================

    /// 中间符号 trait / Intermediate symbol trait
    ///
    /// 连接 function evaluate 与 MetaModel token/约束生成。
    /// Bridge connecting function evaluate to MetaModel token/constraint generation.
    template<typename V = double>
    class IntermediateSymbol {
    public:
        virtual ~IntermediateSymbol() = default;

        /// 获取符号 ID / Get symbol ID
        [[nodiscard]] virtual const IntermediateSymbolId& symbol_id() const noexcept = 0;

        /// 获取符号类别 / Get symbol category
        [[nodiscard]] virtual Category category() const noexcept = 0;

        /// 获取操作类别（默认同 category）/ Get operation category (default: same as category)
        [[nodiscard]] virtual Category operation_category() const noexcept {
            return category();
        }

        /// 是否缓存 / Whether cached
        [[nodiscard]] virtual bool cached() const noexcept = 0;

        /// 获取依赖符号 / Get dependency symbols
        [[nodiscard]] virtual std::vector<std::shared_ptr<IntermediateSymbol>> dependencies() const = 0;

        /// 获取声明的依赖 ID / Get declared dependency IDs
        [[nodiscard]] virtual std::vector<std::uint64_t> declared_dependency_ids() const {
            return {};
        }

        /// 刷新缓存 / Flush cache
        virtual void flush(bool force) = 0;

        /// 注册辅助 token / Register auxiliary tokens
        virtual void register_auxiliary_tokens(std::vector<Token<V>>& /*tokens*/) const {}

        /// 构建机制层线性约束 / Build mechanism-layer linear constraints
        [[nodiscard]] virtual std::vector<LinearConstraint<V>> mechanism_constraints(
            const std::unordered_map<std::size_t, std::size_t>& /*symbol_to_index*/) const
        {
            return {};
        }

        /// 构建机制层线性约束（带 token 上下文）/ Build mechanism-layer linear constraints with token context
        [[nodiscard]] virtual std::vector<LinearConstraint<V>> mechanism_constraints_with_tokens(
            const std::unordered_map<std::size_t, std::size_t>& symbol_to_index,
            const std::vector<Token<V>>& /*tokens*/) const
        {
            return mechanism_constraints(symbol_to_index);
        }

        /// 从 token 表求值 / Evaluate from token table
        [[nodiscard]] virtual std::optional<V> evaluate_from_tokens(
            const TokenListTrait<V>& /*token_table*/,
            bool /*zero_if_none*/) const
        {
            return std::nullopt;
        }

        /// 从值映射准备 / Prepare from value map
        [[nodiscard]] virtual std::optional<V> prepare(
            const std::unordered_map<std::size_t, V>& values) const = 0;

        /// 获取值缓存键 / Get value cache key
        [[nodiscard]] virtual ValueCacheKey evaluation_cache_key() const {
            return ValueCacheKey::from_symbol(symbol_id().id);
        }

        /// 求值（带上下文）/ Evaluate with context
        [[nodiscard]] virtual std::optional<V> evaluate(
            IntermediateSymbolEvalContext<V>& ctx) const
        {
            return evaluate_with_ctx(ctx.values(), ctx.cache());
        }

        /// 求值（带缓存上下文）/ Evaluate with cache context
        [[nodiscard]] virtual std::optional<V> evaluate_with_ctx(
            const std::unordered_map<std::size_t, V>& values,
            ValueCacheContextTrait<V>& cache) const
        {
            auto key = evaluation_cache_key();
            if (auto cached = cache.get(key)) {
                return cached;
            }

            // 预热依赖 / Preheat dependencies
            auto deps = dependencies();
            for (const auto& dep : deps) {
                dep->evaluate_with_ctx(values, cache);
            }

            auto value = evaluate_from_tokens_impl(values).or_else([&]() {
                return prepare(values);
            });

            if (value) {
                cache.set(key, *value);
            }
            return value;
        }

        /// 获取变量范围 / Get variable range
        [[nodiscard]] virtual std::optional<VariableRange<V>> range() const {
            return std::nullopt;
        }

        /// 获取范围缓存键 / Get range cache key
        [[nodiscard]] virtual RangeCacheKey range_cache_key() const {
            return RangeCacheKey::from_symbol(symbol_id().id);
        }

    private:
        /// 从值映射求值（内部实现）/ Evaluate from values (internal)
        [[nodiscard]] virtual std::optional<V> evaluate_from_tokens_impl(
            const std::unordered_map<std::size_t, V>& /*values*/) const
        {
            return std::nullopt;
        }
    };

    // ============================================================================
    // LinearIntermediateSymbol — 线性中间符号 / Linear intermediate symbol
    // ============================================================================

    /// 线性中间符号 trait / Linear intermediate symbol trait
    template<typename V = double>
    class LinearIntermediateSymbol : public virtual IntermediateSymbol<V> {
    public:
        /// 获取线性多项式表示 / Get linear polynomial representation
        [[nodiscard]] virtual std::vector<std::pair<std::size_t, V>> linear_coefficients() const = 0;

        /// 获取常数项 / Get constant term
        [[nodiscard]] virtual V constant_term() const = 0;
    };

    // ============================================================================
    // FunctionSymbol — 函数符号 trait / Function symbol trait
    // ============================================================================

    /// 函数符号 trait / Function symbol trait
    template<typename V = double>
    class FunctionSymbolTrait : public virtual IntermediateSymbol<V> {
    public:
        /// 注册辅助 token / Register auxiliary tokens
        void register_auxiliary_tokens(std::vector<Token<V>>& tokens) const override = 0;

        /// 从 token 表计算函数值 / Calculate function value from token table
        [[nodiscard]] std::optional<V> evaluate_from_tokens(
            const TokenListTrait<V>& token_table,
            bool zero_if_none) const override = 0;
    };

    // ============================================================================
    // 约束关系 / Constraint relation
    // ============================================================================

    /// 约束关系 / Constraint relation
    enum class ConstraintRelation : std::uint8_t {
        LessEqual = 0,
        GreaterEqual = 1,
        Equal = 2,
    };

}  // namespace ospf::core
