/// ospf_framework_csp1d 测试
/// 覆盖 CSP1D 领域模型、切割算法、上下文。

#include <gtest/gtest.h>

#include <ospf/framework/csp1d/domain.hpp>
#include <ospf/framework/csp1d/application.hpp>
#include <ospf/framework/csp1d/context.hpp>

using namespace ospf::framework::csp1d;

// ============================================================================
// Domain 测试 / Domain tests
// ============================================================================

TEST(Domain, Material) {
    Material mat{"steel", "Steel", 5.0};
    EXPECT_EQ(mat.id, "steel");
    EXPECT_EQ(mat.name, "Steel");
    EXPECT_DOUBLE_EQ(mat.unit_cost, 5.0);
}

TEST(Domain, Stock) {
    Material mat{"steel", "Steel", 5.0};
    Stock stock{"s1", mat, 100.0, 10};
    EXPECT_EQ(stock.id, "s1");
    EXPECT_DOUBLE_EQ(stock.length, 100.0);
    EXPECT_EQ(stock.quantity, 10u);
    EXPECT_DOUBLE_EQ(stock.total_length(), 1000.0);
}

TEST(Domain, Piece) {
    Material mat{"steel", "Steel", 5.0};
    Piece piece{"p1", mat, 30.0, 20};
    EXPECT_EQ(piece.id, "p1");
    EXPECT_DOUBLE_EQ(piece.length, 30.0);
    EXPECT_EQ(piece.demand, 20u);
    EXPECT_DOUBLE_EQ(piece.total_length(), 600.0);
}

TEST(Domain, CuttingPattern) {
    CuttingPattern pattern;
    pattern.stock_id = "s1";
    pattern.piece_counts = {3, 2, 1};
    pattern.waste = 10.0;

    std::vector<Piece> pieces;
    Material mat{"steel", "Steel", 5.0};
    pieces.push_back({"p1", mat, 20.0, 10});
    pieces.push_back({"p2", mat, 15.0, 10});
    pieces.push_back({"p3", mat, 10.0, 10});

    EXPECT_DOUBLE_EQ(pattern.used_length(pieces), 100.0);  // 3*20 + 2*15 + 1*10 = 100
}

TEST(Domain, YieldResult) {
    YieldResult result{18, 20, 50.0};
    EXPECT_NEAR(result.yield_rate(), 0.9, 1e-10);
}

// ============================================================================
// Application 测试 / Application tests
// ============================================================================

TEST(Application, FirstFitCuttingName) {
    FirstFitCutting ffc;
    EXPECT_EQ(ffc.name(), "FirstFitCutting");
}

TEST(Application, FirstFitCuttingBasic) {
    FirstFitCutting ffc;

    Material mat{"steel", "Steel", 5.0};
    std::vector<Stock> stocks = {{"s1", mat, 100.0, 5}};
    std::vector<Piece> pieces = {
        {"p1", mat, 30.0, 3},
        {"p2", mat, 20.0, 2}
    };

    auto plan = ffc.generate(stocks, pieces);

    EXPECT_GT(plan.patterns.size(), 0u);
    EXPECT_GE(plan.total_waste, 0.0);
}

TEST(Application, FirstFitCuttingEmpty) {
    FirstFitCutting ffc;
    std::vector<Stock> stocks;
    std::vector<Piece> pieces;

    auto plan = ffc.generate(stocks, pieces);
    EXPECT_EQ(plan.patterns.size(), 0u);
}

TEST(Application, CalculateYield) {
    Material mat{"steel", "Steel", 5.0};
    std::vector<Piece> pieces = {{"p1", mat, 30.0, 10}};

    ProductionPlan plan;
    plan.total_waste = 20.0;

    CuttingPattern pattern;
    pattern.piece_counts = {10};
    plan.patterns.push_back(pattern);

    auto yield = calculate_yield(plan, pieces);
    EXPECT_EQ(yield.produced, 10u);
    EXPECT_EQ(yield.demanded, 10u);
    EXPECT_NEAR(yield.yield_rate(), 1.0, 1e-10);
}

TEST(Application, CalculateWaste) {
    Material mat{"steel", "Steel", 5.0};
    std::vector<Stock> stocks = {{"s1", mat, 100.0, 1}};

    ProductionPlan plan;
    plan.total_waste = 20.0;

    auto waste = calculate_waste(plan, stocks);
    EXPECT_DOUBLE_EQ(waste, 20.0);
}

// ============================================================================
// Context 测试 / Context tests
// ============================================================================

TEST(Csp1dContext, Creation) {
    Csp1dContext ctx;
    EXPECT_EQ(ctx.domain_name(), "CSP1D");
}

TEST(Csp1dContext, AddStock) {
    Csp1dContext ctx;
    Material mat{"steel", "Steel", 5.0};
    ctx.add_stock({"s1", mat, 100.0, 10});

    EXPECT_EQ(ctx.stocks().size(), 1u);
}

TEST(Csp1dContext, AddPiece) {
    Csp1dContext ctx;
    Material mat{"steel", "Steel", 5.0};
    ctx.add_piece({"p1", mat, 30.0, 5});

    EXPECT_EQ(ctx.pieces().size(), 1u);
}

TEST(Csp1dContext, Solve) {
    Csp1dContext ctx;
    Material mat{"steel", "Steel", 5.0};
    ctx.add_stock({"s1", mat, 100.0, 5});
    ctx.add_piece({"p1", mat, 30.0, 3});
    ctx.add_piece({"p2", mat, 20.0, 2});

    FirstFitCutting algorithm;
    auto plan = ctx.solve(algorithm);

    EXPECT_GT(plan.patterns.size(), 0u);
}

// ============================================================================
// 综合测试 / Integration tests
// ============================================================================

TEST(Integration, Csp1dWorkflow) {
    // 创建上下文
    Csp1dContext ctx;

    // 定义材料
    Material steel{"steel", "Steel", 5.0};

    // 添加原料（5 根 100mm 的钢管）
    ctx.add_stock({"stock1", steel, 100.0, 5});

    // 添加需求件
    ctx.add_piece({"piece30", steel, 30.0, 6});  // 需要 6 根 30mm
    ctx.add_piece({"piece20", steel, 20.0, 8});  // 需要 8 根 20mm

    // 执行切割
    FirstFitCutting algorithm;
    auto plan = ctx.solve(algorithm);

    // 验证结果
    EXPECT_GT(plan.patterns.size(), 0u);

    // 计算产出
    auto yield = calculate_yield(plan, ctx.pieces());
    EXPECT_GT(yield.produced, 0u);

    // 计算余料
    auto waste = calculate_waste(plan, ctx.stocks());
    EXPECT_GE(waste, 0.0);
}
