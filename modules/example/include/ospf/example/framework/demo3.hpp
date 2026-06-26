#pragma once
/// Framework Demo3 - 简单 LP 示例 / Simple LP Example
#include <ospf/core/variable.hpp>
#include <ospf/core/model.hpp>
#include <ospf/core/solver.hpp>
namespace ospf::example::framework {
    inline void demo3_simple_lp() {
        using namespace ospf::core;
        SymbolRegistry::instance().reset();
        MetaModel m("simple_lp");
        VariableBuilder b;
        m.add_variable(b.name("x").lower(0).upper(10).build());
        m.add_variable(b.name("y").lower(0).upper(10).build());
        m.set_objective("obj", ObjectiveSense::Maximize,
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(3.0, ospf::math::symbol::Variable("x", 0))) +
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(5.0, ospf::math::symbol::Variable("y", 1))));
        m.add_constraint("c1", ospf::math::symbol::Inequality::less_equal(
            ospf::math::symbol::Expression(ospf::math::symbol::Variable("x", 0)) +
            ospf::math::symbol::Expression(ospf::math::symbol::Variable("y", 1)),
            ospf::math::symbol::Expression(4.0)));
        StubSolver solver;
        auto r = solver.solve(m);
        (void)r;
    }
}
