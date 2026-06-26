#pragma once
/// Demo2 - 产品组合优化 / Product Mix Optimization
/// 确定各产品的生产数量以最大化利润

#include <ospf/core/variable.hpp>
#include <ospf/core/model.hpp>
#include <ospf/core/solver.hpp>

namespace ospf::example::core {

    /// 产品组合优化示例
    inline void demo2() {
        using namespace ospf::core;
        using namespace ospf::math::symbol;

        SymbolRegistry::instance().reset();
        MetaModel model("product_mix");

        VariableBuilder b;
        model.add_variable(b.name("p1").lower(0).build());
        model.add_variable(b.name("p2").lower(0).build());

        // max 5*p1 + 8*p2
        model.set_objective("profit", ObjectiveSense::Maximize,
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(5.0, ospf::math::symbol::Variable("p1", 0))) +
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(8.0, ospf::math::symbol::Variable("p1", 1))));

        // p1 + 2*p2 <= 100
        model.add_constraint("resource",
            ospf::math::symbol::Inequality::less_equal(
                ospf::math::symbol::Expression(ospf::math::symbol::Variable("p1", 0)) + ospf::math::symbol::Expression(ospf::math::symbol::Monomial(2.0, ospf::math::symbol::Variable("p2", 1))),
                ospf::math::symbol::Expression(100.0)));

        StubSolver solver;
        auto result = solver.solve(model);
        (void)result;
    }

}  // namespace ospf::example::core
