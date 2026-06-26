#pragma once
/// Framework Demo1 - 带宽分配 / Bandwidth Allocation
/// 网络带宽优化分配

#include <ospf/core/variable.hpp>
#include <ospf/core/model.hpp>
#include <ospf/core/solver.hpp>

namespace ospf::example::framework {

    inline void demo1_bandwidth() {
        using namespace ospf::core;
        using namespace ospf::math::symbol;

        SymbolRegistry::instance().reset();
        MetaModel model("bandwidth");

        VariableBuilder b;
        model.add_variable(b.name("link1").lower(0).upper(100).build());
        model.add_variable(b.name("link2").lower(0).upper(100).build());

        model.set_objective("throughput", ObjectiveSense::Maximize,
            ospf::math::symbol::Expression(ospf::math::symbol::Variable("link1", 0)) + ospf::math::symbol::Expression(ospf::math::symbol::Variable("link2", 1)));

        model.add_constraint("capacity",
            ospf::math::symbol::Inequality::less_equal(
                ospf::math::symbol::Expression(ospf::math::symbol::Variable("link1", 0)) + ospf::math::symbol::Expression(ospf::math::symbol::Variable("link2", 1)),
                ospf::math::symbol::Expression(150.0)));

        StubSolver solver;
        auto result = solver.solve(model);
        (void)result;
    }

}  // namespace ospf::example::framework
