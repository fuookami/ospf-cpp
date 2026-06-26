#pragma once
/// Demo9 - 示例 9 / Example 9
#include <ospf/core/variable.hpp>
#include <ospf/core/model.hpp>
#include <ospf/core/solver.hpp>
namespace ospf::example::core {
    inline void demo9() {
        using namespace ospf::core;
        SymbolRegistry::instance().reset();
        MetaModel m("demo9");
        VariableBuilder b;
        m.add_variable(b.name("x").lower(0).upper(10).build());
        m.set_objective("obj", ObjectiveSense::Maximize,
            ospf::math::symbol::Expression(ospf::math::symbol::Variable("x", 0)));
        StubSolver solver;
        auto r = solver.solve(m);
        (void)r;
    }
}
