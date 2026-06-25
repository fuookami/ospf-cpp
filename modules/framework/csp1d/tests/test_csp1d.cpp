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

// ============================================================================
// 列生成探针测试 / Column generation probe tests
// 对齐 Rust csp1d_migration.rs: column_generation_applies_initial_plan_flow_policy_filter
// ============================================================================

#include <ospf/framework/csp1d/application/service.hpp>

using namespace ospf::framework::csp1d;

/// 过滤流程策略 / Filtering flow policy
/// 对应 Rust 测试中的 FilteringFlowPolicy
/// 过滤掉指定 ID 的方案
class FilteringFlowPolicy : public Csp1dFlowPolicy {
public:
    explicit FilteringFlowPolicy(std::string rejected_plan_id)
        : rejected_plan_id_(std::move(rejected_plan_id)) {}

    [[nodiscard]] const std::string& name() const override {
        static const std::string n = "filtering_flow_policy";
        return n;
    }

    /// 过滤初始方案：移除 id == rejected_plan_id_ 的方案
    /// 对应 Rust FilteringFlowPolicy::filter_initial_plans
    [[nodiscard]] std::vector<CuttingPlan> filter_initial_plans(
        const Csp1dFlowContext& /*context*/,
        std::vector<CuttingPlan> plans) const override
    {
        std::vector<CuttingPlan> result;
        for (auto& plan : plans) {
            if (plan.id != rejected_plan_id_) {
                result.push_back(std::move(plan));
            }
        }
        return result;
    }

    /// 材料 ID 相同即等价 / Same material ID = equivalent
    /// 对应 Rust FilteringFlowPolicy::is_equivalent
    [[nodiscard]] bool is_equivalent(
        const Csp1dFlowContext& /*context*/,
        const CuttingPlan& existing,
        const CuttingPlan& candidate) const override
    {
        return existing.material_id == candidate.material_id;
    }

    /// 不接受部分方案 / Don't accept partial
    /// 对应 Rust FilteringFlowPolicy::accept_partial
    [[nodiscard]] bool accept_partial(
        const Csp1dFlowContext& /*context*/) const override
    {
        return false;
    }

private:
    std::string rejected_plan_id_;
};

// ============================================================================
// 差分对齐测试 / Differential alignment test
// 对齐 Rust: column_generation_applies_initial_plan_flow_policy_filter
// 参考 Rust 输出：kept("keep-me") 保留，dropped("drop-me") 被过滤
// ============================================================================

TEST(ColumnGeneration, FlowPolicyFiltersInitialPlans) {
    // 构造两个切割方案 / Create two cutting plans
    CuttingPlan kept;
    kept.id = "keep-me";
    kept.material_id = "m1";
    kept.slices = {{"p1", 30.0, 2}};

    CuttingPlan dropped;
    dropped.id = "drop-me";
    dropped.material_id = "m1";
    dropped.slices = {{"p1", 20.0, 2}};

    // 创建过滤策略：拒绝 "drop-me" / Create filter policy: reject "drop-me"
    auto policy = std::make_shared<FilteringFlowPolicy>("drop-me");

    // 创建固定方案枚举器 + 空定价生成器 / Fixed enumerator + empty pricing
    auto enumerator = std::make_shared<FixedPlanEnumerator>(
        std::vector<CuttingPlan>{kept, dropped});
    auto pricing = std::make_shared<EmptyPricingGenerator>();

    // 创建列生成服务 / Create column generation service
    Csp1dColumnGeneration service(
        enumerator,
        pricing,
        std::vector<std::shared_ptr<Csp1dFlowPolicy>>{policy});

    // 执行 / Execute
    auto result = service.solve();

    // 差分对齐断言（与 Rust 参考一致）/ Differential alignment assertions
    // Rust 断言：result.solution.generated_plans 包含 kept，不包含 dropped
    bool has_kept = false;
    bool has_dropped = false;
    for (const auto& plan : result.generated_plans) {
        if (plan.id == "keep-me") has_kept = true;
        if (plan.id == "drop-me") has_dropped = true;
    }

    EXPECT_TRUE(has_kept) << "kept plan 'keep-me' should be retained after flow policy filter";
    EXPECT_FALSE(has_dropped) << "dropped plan 'drop-me' should be filtered out by flow policy";

    // 验证终止原因 / Verify termination reason
    EXPECT_EQ(result.termination_reason, "PricingConverged");
    EXPECT_EQ(result.iteration_count, 0);
}

// ============================================================================
// 差分对齐测试：列生成初始流程策略接收上下文
// 对齐 Rust: column_generation_initial_flow_policy_receives_kotlin_like_context
// ============================================================================

TEST(ColumnGeneration, FlowPolicyReceivesContext) {
    // 验证 flow policy 能访问流程上下文 / Verify flow policy can access context
    struct InspectingPolicy : public Csp1dFlowPolicy {
        mutable bool observed = false;

        [[nodiscard]] const std::string& name() const override {
            static const std::string n = "inspecting";
            return n;
        }

        [[nodiscard]] std::vector<CuttingPlan> filter_initial_plans(
            const Csp1dFlowContext& context,
            std::vector<CuttingPlan> plans) const override
        {
            // 验证上下文包含迭代信息 / Verify context has iteration info
            observed = (context.iteration == 0);
            return plans;
        }

        [[nodiscard]] bool is_equivalent(
            const Csp1dFlowContext&, const CuttingPlan&, const CuttingPlan&) const override {
            return false;
        }

        [[nodiscard]] bool accept_partial(const Csp1dFlowContext&) const override {
            return true;
        }
    };

    auto policy = std::make_shared<InspectingPolicy>();

    CuttingPlan plan;
    plan.id = "test-plan";
    plan.material_id = "m1";
    plan.slices = {{"p1", 30.0, 1}};

    auto enumerator = std::make_shared<FixedPlanEnumerator>(
        std::vector<CuttingPlan>{plan});
    auto pricing = std::make_shared<EmptyPricingGenerator>();

    Csp1dColumnGeneration service(
        enumerator, pricing,
        std::vector<std::shared_ptr<Csp1dFlowPolicy>>{policy});

    auto result = service.solve();

    // 验证 flow policy 观察到上下文 / Verify flow policy observed context
    EXPECT_TRUE(policy->observed) << "flow policy should have received context with iteration=0";
    EXPECT_EQ(result.generated_plans.size(), 1u);
    EXPECT_EQ(result.generated_plans[0].id, "test-plan");
}
