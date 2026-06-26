#pragma once
/// Framework Demo4 - 航班排程 / Flight Scheduling
/// 航班机组排班优化

#include <ospf/core/variable.hpp>
#include <ospf/core/model.hpp>
#include <ospf/core/solver.hpp>

namespace ospf::example::framework {

    inline void demo4_flight() {
        using namespace ospf::core;
        using namespace ospf::math::symbol;

        SymbolRegistry::instance().reset();
        MetaModel model("flight_schedule");

        VariableBuilder b;
        // 3 个航班，2 个机组
        model.add_variable(b.name("f1_c1").type(VariableType::Binary).build());
        model.add_variable(b.name("f1_c2").type(VariableType::Binary).build());
        model.add_variable(b.name("f2_c1").type(VariableType::Binary).build());
        model.add_variable(b.name("f2_c2").type(VariableType::Binary).build());
        model.add_variable(b.name("f3_c1").type(VariableType::Binary).build());
        model.add_variable(b.name("f3_c2").type(VariableType::Binary).build());

        // 每个航班恰好一个机组
        model.add_constraint("f1", ospf::math::symbol::Inequality::less_equal(
            ospf::math::symbol::Expression(ospf::math::symbol::Variable("f1_c1", 0)) + ospf::math::symbol::Expression(ospf::math::symbol::Variable("f1_c2", 1)),
            ospf::math::symbol::Expression(1.0)));

        model.add_constraint("f2", ospf::math::symbol::Inequality::less_equal(
            ospf::math::symbol::Expression(ospf::math::symbol::Variable("f2_c1", 2)) + ospf::math::symbol::Expression(ospf::math::symbol::Variable("f2_c2", 3)),
            ospf::math::symbol::Expression(1.0)));

        model.add_constraint("f3", ospf::math::symbol::Inequality::less_equal(
            ospf::math::symbol::Expression(ospf::math::symbol::Variable("f3_c1", 4)) + ospf::math::symbol::Expression(ospf::math::symbol::Variable("f3_c2", 5)),
            ospf::math::symbol::Expression(1.0)));

        // 最小化总成本
        model.set_objective("cost", ObjectiveSense::Minimize,
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(10.0, ospf::math::symbol::Variable("f1_c1", 0))) +
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(15.0, ospf::math::symbol::Variable("f1_c2", 1))) +
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(12.0, ospf::math::symbol::Variable("f2_c1", 2))) +
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(8.0, ospf::math::symbol::Variable("f2_c2", 3))) +
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(20.0, ospf::math::symbol::Variable("f3_c1", 4))) +
            ospf::math::symbol::Expression(ospf::math::symbol::Monomial(10.0, ospf::math::symbol::Variable("f3_c2", 5))));

        StubSolver solver;
        auto result = solver.solve(model);
        (void)result;
    }

}  // namespace ospf::example::framework
