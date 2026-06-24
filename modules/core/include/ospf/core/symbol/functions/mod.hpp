#pragma once
/// 符号函数聚合头 / Symbol function aggregation header
/// 1:1 对应 Rust core/symbol/functions/mod.rs（兼容入口）

#include <ospf/core/symbol/functions/abs.hpp>
#include <ospf/core/symbol/functions/and.hpp>
#include <ospf/core/symbol/functions/balance_ternaryzation.hpp>
#include <ospf/core/symbol/functions/big_m.hpp>
#include <ospf/core/symbol/functions/binaryzation.hpp>
#include <ospf/core/symbol/functions/bivariate_linear_piecewise.hpp>
#include <ospf/core/symbol/functions/ceiling.hpp>
#include <ospf/core/symbol/functions/cos.hpp>
#include <ospf/core/symbol/functions/first.hpp>
#include <ospf/core/symbol/functions/floor.hpp>
#include <ospf/core/symbol/functions/if_function.hpp>
#include <ospf/core/symbol/functions/if_in.hpp>
#include <ospf/core/symbol/functions/if_then.hpp>
#include <ospf/core/symbol/functions/imply.hpp>
#include <ospf/core/symbol/functions/in_step_range.hpp>
#include <ospf/core/symbol/functions/inequality.hpp>
#include <ospf/core/symbol/functions/masking.hpp>
#include <ospf/core/symbol/functions/max.hpp>
#include <ospf/core/symbol/functions/min_max.hpp>
#include <ospf/core/symbol/functions/mod_function.hpp>
#include <ospf/core/symbol/functions/one_of.hpp>
#include <ospf/core/symbol/functions/p0_evaluation_tests.hpp>
#include <ospf/core/symbol/functions/product.hpp>
#include <ospf/core/symbol/functions/quadratic_function.hpp>
#include <ospf/core/symbol/functions/quadratic_in_step_range.hpp>
#include <ospf/core/symbol/functions/quadratic_linear.hpp>
#include <ospf/core/symbol/functions/quadratic_masking_range.hpp>
#include <ospf/core/symbol/functions/quadratic_min.hpp>
#include <ospf/core/symbol/functions/rounding.hpp>
#include <ospf/core/symbol/functions/same_as.hpp>
#include <ospf/core/symbol/functions/satisfied_amount.hpp>
#include <ospf/core/symbol/functions/satisfied_amount_inequality.hpp>
#include <ospf/core/symbol/functions/semantic.hpp>
#include <ospf/core/symbol/functions/semi.hpp>
#include <ospf/core/symbol/functions/sigmoid.hpp>
#include <ospf/core/symbol/functions/sin.hpp>
#include <ospf/core/symbol/functions/slack.hpp>
#include <ospf/core/symbol/functions/slack_range.hpp>
#include <ospf/core/symbol/functions/univariate_linear_piecewise.hpp>

#include <ospf/core/symbol/function_symbol.hpp>
#include <cmath>
#include <optional>

namespace ospf::core {

    /// mod 符号函数 / mod symbol function
    /// 1:1 对应 Rust core/symbol/functions/mod.rs
    template<typename V = double>
    class ModFunctionSimple : public BinaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "mod"; }

        [[nodiscard]] std::optional<V> evaluate_binary(V a, V b) const override {
            if (b == V{0}) return std::nullopt;
            if constexpr (std::is_integral_v<V>) { return a % b; }
            else { return std::fmod(a, b); }
        }
    };

}  // namespace ospf::core
