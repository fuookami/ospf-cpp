#pragma once
/// Demo1 - 投资组合选择问题 / Portfolio Selection Problem
/// 选择一组公司使得总利润最大化，满足资本和负债约束

#include <ospf/core/variable.hpp>
#include <ospf/core/model.hpp>
#include <ospf/core/solver.hpp>
#include <string>
#include <vector>

namespace ospf::example::core {

    struct Company {
        std::string name;
        double capital;
        double liability;
        double profit;
    };

    /// 投资组合选择示例 / Portfolio selection example
    inline void demo1() {
        using namespace ospf::core;
        using namespace ospf::math::symbol;

        SymbolRegistry::instance().reset();
        MetaModel model("portfolio");

        std::vector<Company> companies = {
            {"A", 5.0, 2.0, 10.0},
            {"B", 3.0, 1.0, 8.0},
            {"C", 4.0, 3.0, 12.0},
            {"D", 2.0, 1.0, 6.0},
            {"E", 6.0, 2.0, 15.0}
        };

        VariableBuilder b;
        for (std::size_t i = 0; i < companies.size(); ++i) {
            model.add_variable(b.name("x" + std::to_string(i))
                .type(VariableType::Binary).build());
        }

        // 目标：最大化总利润
        Expression obj(0.0);
        for (std::size_t i = 0; i < companies.size(); ++i) {
            obj = obj + ospf::math::symbol::Expression(ospf::math::symbol::Monomial(companies[i].profit, ospf::math::symbol::Variable("x" + std::to_string(i), i)));
        }
        model.set_objective("profit", ObjectiveSense::Maximize, obj);

        // 约束：总资本 >= 10
        // 约束：总负债 <= 5
        StubSolver solver;
        auto result = solver.solve(model);
        (void)result;
    }

}  // namespace ospf::example::core
