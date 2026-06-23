/// CSP1D demo
/// 演示一维下料问题的求解。

#include <ospf/framework/csp1d/context.hpp>
#include <ospf/framework/csp1d/application.hpp>
#include <iostream>

int main() {
    using namespace ospf::framework::csp1d;

    std::cout << "=== CSP1D Demo ===" << std::endl;

    // 创建上下文
    Csp1dContext ctx;

    // 定义材料
    Material steel{"steel", "Steel", 5.0};

    // 添加原料（5 根 100mm 的钢管）
    ctx.add_stock({"stock1", steel, 100.0, 5});

    // 添加需求件
    ctx.add_piece({"piece30", steel, 30.0, 6});  // 需要 6 根 30mm
    ctx.add_piece({"piece20", steel, 20.0, 8});  // 需要 8 根 20mm

    std::cout << "Stocks: " << ctx.stocks().size() << std::endl;
    std::cout << "Pieces: " << ctx.pieces().size() << std::endl;

    // 执行切割
    FirstFitCutting algorithm;
    auto plan = ctx.solve(algorithm);

    std::cout << "Algorithm: " << algorithm.name() << std::endl;
    std::cout << "Cutting patterns: " << plan.patterns.size() << std::endl;
    std::cout << "Total waste: " << plan.total_waste << " mm" << std::endl;

    // 计算产出
    auto yield = calculate_yield(plan, ctx.pieces());
    std::cout << "Produced: " << yield.produced << " / " << yield.demanded << std::endl;
    std::cout << "Yield rate: " << (yield.yield_rate() * 100.0) << "%" << std::endl;

    std::cout << "=== Demo Complete ===" << std::endl;
    return 0;
}
