#pragma once
/// Demo3 - 资源分配 / Resource Allocation
/// 将有限资源分配给多个项目以最大化收益

#include <ospf/core/variable.hpp>
#include <ospf/core/model.hpp>
#include <ospf/core/solver.hpp>

namespace ospf::example::core {

    /// 资源分配示例
    inline void demo3() {
        using namespace ospf::core;
        using namespace ospf::math::symbol;

        SymbolRegistry::instance().reset();
        MetaModel model("resource_alloc");

        VariableBuilder b;
        model.add_variable(b.name("a").lower(0).upper(10).build());
        model.add_variable(b.name("b").lower(0).upper(10).build());
        model.add_variable(b.name("c").lower(0).upper(10).build());

        // max 3a + 5b + 2c
        model.set_objective("收益", ObjectiveSense::Maximize,
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(3.0, ospf::math::symbol::Variable("a", 0))) +
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(5.0, ospf::math::symbol::Variable("b", 1))) +
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(2.0, ospf::math::symbol::Variable("c", 2))));

        // a + b + c <= 20
        model.add_constraint("资源约束",
            ospf::math::symbol::Inequality::less_equal(
                ospf::math::symbol::Expression(ospf::math::symbol::Variable("a", 0)) + ospf::math::symbol::Expression(ospf::math::symbol::Variable("b", 1)) + ospf::math::symbol::Expression(ospf::math::symbol::Variable("c", 2)),
                ospf::math::symbol::Expression(20.0)));

        StubSolver solver;
        auto result = solver.solve(model);
        (void)result;
    }

}  // namespace ospf::example::core
