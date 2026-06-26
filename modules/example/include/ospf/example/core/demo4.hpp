#pragma once
/// Demo4 - 资源分配优化 / Resource Allocation Optimization
#include <ospf/core/variable.hpp>
#include <ospf/core/model.hpp>
#include <ospf/core/solver.hpp>
namespace ospf::example::core {
    inline void demo4() {
        using namespace ospf::core;
        SymbolRegistry::instance().reset();
        MetaModel m("resource_alloc");
        VariableBuilder b;
        m.add_variable(b.name("x").lower(0).upper(100).build());
        m.add_variable(b.name("y").lower(0).upper(100).build());
        m.set_objective("obj", ObjectiveSense::Maximize,
            ospf::math::symbol::Expression(ospf::math::symbol::Variable("x", 0)) +
            ospf::math::symbol::Expression(ospf::math::symbol::Variable("y", 1)));
        StubSolver solver;
        auto r = solver.solve(m);
        (void)r;
    }
}
