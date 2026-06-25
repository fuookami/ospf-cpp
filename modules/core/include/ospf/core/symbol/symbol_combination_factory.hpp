#pragma once
/// SymbolCombinationFactory — 符号组合工厂 / Symbol combination factory
/// 1:1 对应 Rust core/symbol/symbol_combination_factory.rs

#include <ospf/core/symbol/symbol_combination.hpp>
#include <ospf/core/symbol/expression_symbol.hpp>
#include <ospf/core/symbol/monomial_cell.hpp>
#include <memory>
#include <vector>

namespace ospf::core {

    /// 符号组合工厂 / Symbol combination factory
    ///
    /// 提供便捷方法创建各种符号组合。
    /// Provides convenience methods for creating various symbol combinations.
    template<typename V = double>
    class SymbolCombinationFactory {
    public:
        /// 创建线性表达式 / Create linear expression
        [[nodiscard]] static std::shared_ptr<ExpressionSymbol<V>> create_expression(
            std::vector<typename ExpressionSymbol<V>::Monomial> monomials,
            V constant = V{})
        {
            auto id = IntermediateSymbolId::from_id(next_auto_intermediate_symbol_id());
            return std::make_shared<ExpressionSymbol<V>>(
                std::move(id), std::move(monomials), constant);
        }

        /// 创建单项式 / Create monomial
        [[nodiscard]] static std::shared_ptr<MonomialCell<V>> create_monomial(
            V coefficient, std::size_t variable_index, V exponent = V{1})
        {
            auto id = IntermediateSymbolId::from_id(next_auto_intermediate_symbol_id());
            return std::make_shared<MonomialCell<V>>(
                std::move(id), coefficient, variable_index, exponent);
        }

        /// 创建符号组合 / Create symbol combination
        [[nodiscard]] static std::shared_ptr<SymbolCombination<V>> create_combination(
            std::vector<std::shared_ptr<IntermediateSymbol<V>>> symbols)
        {
            auto id = IntermediateSymbolId::from_id(next_auto_intermediate_symbol_id());
            return std::make_shared<SymbolCombination<V>>(
                std::move(id), std::move(symbols));
        }

        /// 创建变量符号 / Create variable symbol
        [[nodiscard]] static std::shared_ptr<MonomialCell<V>> create_variable(
            std::size_t variable_index)
        {
            return create_monomial(V{1}, variable_index, V{1});
        }

        /// 创建常数符号 / Create constant symbol
        [[nodiscard]] static std::shared_ptr<ExpressionSymbol<V>> create_constant(V value) {
            return create_expression({}, value);
        }
    };

}  // namespace ospf::core
