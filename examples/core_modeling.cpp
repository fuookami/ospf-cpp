/// Core 建模 demo
/// 演示 MetaModel、变量、约束、目标的使用。

#include <ospf/core/symbol.hpp>
#include <ospf/core/variable.hpp>
#include <ospf/core/model.hpp>
#include <ospf/core/solver.hpp>
#include <iostream>

int main() {
    using namespace ospf::core;
    namespace sym = ospf::math::symbol;

    std::cout << "=== Core Modeling Demo ===" << std::endl;

    // 重置符号注册表
    SymbolRegistry::instance().reset();

    // 创建模型
    MetaModel model("production_planning");

    // 注册变量
    VariableBuilder builder;
    auto x1 = builder.name("x1").lower(0).upper(100).build();
    auto x2 = builder.name("x2").lower(0).upper(80).build();
    model.add_variable(std::move(x1));
    model.add_variable(std::move(x2));

    std::cout << "Variables: " << model.variable_count() << std::endl;

    // 添加约束：x1 + x2 <= 120
    model.add_constraint("capacity",
        sym::Inequality::less_equal(
            sym::Expression(sym::Variable("x1", 1)) + sym::Expression(sym::Variable("x2", 2)),
            sym::Expression(120.0)
        )
    );

    std::cout << "Constraints: " << model.constraint_count() << std::endl;

    // 设置目标：最大化 3*x1 + 5*x2
    model.set_objective("profit", ObjectiveSense::Maximize,
        sym::Expression(sym::Monomial(3.0, sym::Variable("x1", 1))) +
        sym::Expression(sym::Monomial(5.0, sym::Variable("x2", 2)))
    );

    std::cout << "Objective: maximize 3*x1 + 5*x2" << std::endl;
    std::cout << "Model valid: " << (model.is_valid() ? "yes" : "no") << std::endl;

    // 使用 stub solver 求解
    StubSolver solver;
    auto result = solver.solve(model);

    std::cout << "Status: ";
    switch (result.status) {
        case SolveStatus::Optimal: std::cout << "Optimal"; break;
        case SolveStatus::Feasible: std::cout << "Feasible"; break;
        default: std::cout << "Other"; break;
    }
    std::cout << std::endl;

    if (result.objective_value.has_value()) {
        std::cout << "Objective value: " << *result.objective_value << std::endl;
    }

    std::cout << "Variable values:" << std::endl;
    for (const auto& [name, val] : result.variable_values) {
        std::cout << "  " << name << " = " << val << std::endl;
    }

    std::cout << "=== Demo Complete ===" << std::endl;
    return 0;
}
