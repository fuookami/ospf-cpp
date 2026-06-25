#pragma once
/// ExpressionSymbol — 表达式符号 / Expression symbol
/// 1:1 对应 Rust core/symbol/expression_symbol.rs

#include <ospf/core/symbol/intermediate_symbol.hpp>
#include <string>
#include <vector>
#include <optional>

namespace ospf::core {

    /// 表达式符号 / Expression symbol
    ///
    /// 表示一个由多个单项式组成的线性表达式。
    /// Represents a linear expression composed of multiple monomials.
    template<typename V = double>
    class ExpressionSymbol : public LinearIntermediateSymbol<V> {
    public:
        /// 单项式 / Monomial
        struct Monomial {
            V coefficient;
            std::size_t variable_index;
        };

        ExpressionSymbol(
            IntermediateSymbolId id,
            std::vector<Monomial> monomials,
            V constant = V{}
        ) : id_(std::move(id)), monomials_(std::move(monomials)), constant_(constant) {}

        [[nodiscard]] const IntermediateSymbolId& symbol_id() const noexcept override {
            return id_;
        }

        [[nodiscard]] Category category() const noexcept override {
            return Category::Linear;
        }

        [[nodiscard]] bool cached() const noexcept override { return true; }

        [[nodiscard]] std::vector<std::shared_ptr<IntermediateSymbol<V>>> dependencies() const override {
            return {};
        }

        void flush(bool /*force*/) override {}

        [[nodiscard]] std::optional<V> prepare(
            const std::unordered_map<std::size_t, V>& values) const override
        {
            V result = constant_;
            for (const auto& mono : monomials_) {
                auto it = values.find(mono.variable_index);
                if (it != values.end()) {
                    result += mono.coefficient * it->second;
                }
            }
            return result;
        }

        [[nodiscard]] std::vector<std::pair<std::size_t, V>> linear_coefficients() const override {
            std::vector<std::pair<std::size_t, V>> result;
            for (const auto& mono : monomials_) {
                result.emplace_back(mono.variable_index, mono.coefficient);
            }
            return result;
        }

        [[nodiscard]] V constant_term() const override { return constant_; }

        [[nodiscard]] const std::vector<Monomial>& monomials() const noexcept { return monomials_; }

    private:
        IntermediateSymbolId id_;
        std::vector<Monomial> monomials_;
        V constant_;
    };

}  // namespace ospf::core
