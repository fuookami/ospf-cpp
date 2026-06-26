#pragma once
/// Framework Demo2 - 航空装载优化 / Aviation Loading Optimization
#include <ospf/core/variable.hpp>
#include <ospf/core/model.hpp>
#include <ospf/core/solver.hpp>
namespace ospf::example::framework {
    inline void demo2_loading() {
        using namespace ospf::core;
        SymbolRegistry::instance().reset();
        MetaModel m("loading");
        VariableBuilder b;
        m.add_variable(b.name("cargo1").lower(0).build());
        m.add_variable(b.name("cargo2").lower(0).build());
        m.set_objective("weight", ObjectiveSense::Maximize,
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(100.0, ospf::math::symbol::Variable("cargo1", 0))) +
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(200.0, ospf::math::symbol::Variable("cargo2", 1))));
        m.add_constraint("capacity", ospf::math::symbol::Inequality::less_equal(
            ospf::math::symbol::Expression(ospf::math::symbol::Variable("cargo1", 0)) +
            ospf::math::symbol::Expression(ospf::math::symbol::Variable("cargo2", 1)),
            ospf::math::symbol::Expression(500.0)));
        StubSolver solver;
        auto r = solver.solve(m);
        (void)r;
    }
}
