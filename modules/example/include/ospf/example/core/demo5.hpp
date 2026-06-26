#pragma once
/// Demo5 - 背包问题 / Knapsack Problem
#include <ospf/core/variable.hpp>
#include <ospf/core/model.hpp>
#include <ospf/core/solver.hpp>
namespace ospf::example::core {
    inline void demo5() {
        using namespace ospf::core;
        SymbolRegistry::instance().reset();
        MetaModel m("knapsack");
        VariableBuilder b;
        m.add_variable(b.name("item1").type(VariableType::Binary).build());
        m.add_variable(b.name("item2").type(VariableType::Binary).build());
        m.add_variable(b.name("item3").type(VariableType::Binary).build());
        m.set_objective("value", ObjectiveSense::Maximize,
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(10.0, ospf::math::symbol::Variable("item1", 0))) +
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(20.0, ospf::math::symbol::Variable("item2", 1))) +
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(15.0, ospf::math::symbol::Variable("item3", 2))));
        m.add_constraint("weight", ospf::math::symbol::Inequality::less_equal(
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(5.0, ospf::math::symbol::Variable("item1", 0))) +
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(10.0, ospf::math::symbol::Variable("item2", 1))) +
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(8.0, ospf::math::symbol::Variable("item3", 2))),
            ospf::math::symbol::Expression(15.0)));
        StubSolver solver;
        auto r = solver.solve(m);
        (void)r;
    }
}
