#pragma once
/// ·ûºÅº¯Êý: slack
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/slack.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class SlackFunction : public UnaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "slack"; }

        [[nodiscard]] std::optional<V> evaluate_single(V input) const override {
            return std::max(V{0}, input);
        }
    };

}  // namespace ospf::core
