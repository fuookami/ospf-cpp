#pragma once
/// SymbolCombination — 符号组合 / Symbol combination
/// 1:1 对应 Rust core/symbol/symbol_combination.rs

#include <ospf/core/symbol/intermediate_symbol.hpp>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

namespace ospf::core {

    /// 符号组合 / Symbol combination
    ///
    /// 将多个中间符号组合成一个复合符号。
    /// Combines multiple intermediate symbols into a composite symbol.
    template<typename V = double>
    class SymbolCombination : public IntermediateSymbol<V> {
    public:
        SymbolCombination(
            IntermediateSymbolId id,
            std::vector<std::shared_ptr<IntermediateSymbol<V>>> symbols
        ) : id_(std::move(id)), symbols_(std::move(symbols)) {}

        [[nodiscard]] const IntermediateSymbolId& symbol_id() const noexcept override {
            return id_;
        }

        [[nodiscard]] Category category() const noexcept override {
            // 返回最高类别 / Return highest category
            Category max_cat = Category::Linear;
            for (const auto& sym : symbols_) {
                auto cat = sym->category();
                if (static_cast<int>(cat) > static_cast<int>(max_cat)) {
                    max_cat = cat;
                }
            }
            return max_cat;
        }

        [[nodiscard]] bool cached() const noexcept override { return true; }

        [[nodiscard]] std::vector<std::shared_ptr<IntermediateSymbol<V>>> dependencies() const override {
            return symbols_;
        }

        void flush(bool force) override {
            for (auto& sym : symbols_) {
                sym->flush(force);
            }
        }

        [[nodiscard]] std::optional<V> prepare(
            const std::unordered_map<std::size_t, V>& values) const override
        {
            // 组合符号的值是所有子符号值的和
            // Value of combination is sum of all sub-symbol values
            V result = V{};
            for (const auto& sym : symbols_) {
                auto val = sym->prepare(values);
                if (!val) return std::nullopt;
                result += *val;
            }
            return result;
        }

        [[nodiscard]] const std::vector<std::shared_ptr<IntermediateSymbol<V>>>& symbols() const noexcept {
            return symbols_;
        }

    private:
        IntermediateSymbolId id_;
        std::vector<std::shared_ptr<IntermediateSymbol<V>>> symbols_;
    };

}  // namespace ospf::core
