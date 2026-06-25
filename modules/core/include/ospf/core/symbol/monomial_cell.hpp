#pragma once
/// MonomialCell — 单项式单元 / Monomial cell
/// 1:1 对应 Rust core/symbol/monomial_cell.rs

#include <ospf/core/symbol/intermediate_symbol.hpp>
#include <string>
#include <optional>

namespace ospf::core {

    /// 单项式单元 / Monomial cell
    ///
    /// 表示一个变量乘以系数的单项式。
    /// Represents a monomial: coefficient * variable^exponent.
    template<typename V = double>
    class MonomialCell : public LinearIntermediateSymbol<V> {
    public:
        MonomialCell(
            IntermediateSymbolId id,
            V coefficient,
            std::size_t variable_index,
            V exponent = V{1}
        ) : id_(std::move(id))
          , coefficient_(coefficient)
          , variable_index_(variable_index)
          , exponent_(exponent) {}

        [[nodiscard]] const IntermediateSymbolId& symbol_id() const noexcept override {
            return id_;
        }

        [[nodiscard]] Category category() const noexcept override {
            return (exponent_ == V{1}) ? Category::Linear : Category::Quadratic;
        }

        [[nodiscard]] bool cached() const noexcept override { return true; }

        [[nodiscard]] std::vector<std::shared_ptr<IntermediateSymbol<V>>> dependencies() const override {
            return {};
        }

        void flush(bool /*force*/) override {}

        [[nodiscard]] std::optional<V> prepare(
            const std::unordered_map<std::size_t, V>& values) const override
        {
            auto it = values.find(variable_index_);
            if (it == values.end()) return std::nullopt;

            if (exponent_ == V{1}) {
                return coefficient_ * it->second;
            }
            // 使用 std::pow 处理非整数指数 / Use std::pow for non-integer exponents
            return coefficient_ * std::pow(static_cast<double>(it->second),
                                           static_cast<double>(exponent_));
        }

        [[nodiscard]] std::vector<std::pair<std::size_t, V>> linear_coefficients() const override {
            if (exponent_ == V{1}) {
                return {{variable_index_, coefficient_}};
            }
            return {};
        }

        [[nodiscard]] V constant_term() const override { return V{}; }

        [[nodiscard]] V coefficient() const noexcept { return coefficient_; }
        [[nodiscard]] std::size_t variable_index() const noexcept { return variable_index_; }
        [[nodiscard]] V exponent() const noexcept { return exponent_; }

    private:
        IntermediateSymbolId id_;
        V coefficient_;
        std::size_t variable_index_;
        V exponent_;
    };

}  // namespace ospf::core
