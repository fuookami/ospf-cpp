/// ospf_framework_bpp3d 测试
/// 覆盖 BPP3D 领域模型、打包算法、上下文。

#include <gtest/gtest.h>

#include <ospf/framework/bpp3d/domain.hpp>
#include <ospf/framework/bpp3d/application.hpp>
#include <ospf/framework/bpp3d/context.hpp>

using namespace ospf::framework::bpp3d;

// ============================================================================
// Domain 测试 / Domain tests
// ============================================================================

TEST(Domain, Dimensions) {
    Dimensions d{3.0, 4.0, 5.0};
    EXPECT_DOUBLE_EQ(d.volume(), 60.0);
}

TEST(Domain, Item) {
    Item item{"item1", {2.0, 3.0, 4.0}, 10.0};
    EXPECT_EQ(item.id, "item1");
    EXPECT_DOUBLE_EQ(item.volume(), 24.0);
    EXPECT_DOUBLE_EQ(item.weight, 10.0);
}

TEST(Domain, Bin) {
    Bin bin{"bin1", {10.0, 10.0, 10.0}, 100.0};
    EXPECT_EQ(bin.id, "bin1");
    EXPECT_DOUBLE_EQ(bin.volume(), 1000.0);
    EXPECT_DOUBLE_EQ(bin.max_weight, 100.0);
}

TEST(Domain, PlacedItemOverlap) {
    PlacedItem a{{"a", {2.0, 2.0, 2.0}}, {0.0, 0.0, 0.0}};
    PlacedItem b{{"b", {2.0, 2.0, 2.0}}, {1.0, 1.0, 1.0}};
    PlacedItem c{{"c", {2.0, 2.0, 2.0}}, {5.0, 5.0, 5.0}};

    EXPECT_TRUE(a.overlaps(b));
    EXPECT_FALSE(a.overlaps(c));
}

TEST(Domain, Layer) {
    Layer layer;
    layer.height = 0.0;
    PlacedItem a{{"a", {2.0, 3.0, 4.0}}, {0.0, 0.0, 0.0}};
    PlacedItem b{{"b", {2.0, 3.0, 4.0}}, {2.0, 0.0, 0.0}};
    layer.items.push_back(a);
    layer.items.push_back(b);

    EXPECT_DOUBLE_EQ(layer.used_area(), 16.0);  // 2*4 + 2*4 = 16
}

TEST(Domain, PackingSolution) {
    PackingSolution solution;
    solution.bin_id = "bin1";
    PlacedItem a{{"a", {2.0, 3.0, 4.0}}, {0.0, 0.0, 0.0}};
    PlacedItem b{{"b", {2.0, 3.0, 4.0}}, {2.0, 0.0, 0.0}};
    solution.placed_items.push_back(a);
    solution.placed_items.push_back(b);

    EXPECT_DOUBLE_EQ(solution.total_volume(), 48.0);  // 24 + 24
    EXPECT_NEAR(solution.utilization(1000.0), 0.048, 1e-10);
}

// ============================================================================
// Application 测试 / Application tests
// ============================================================================

TEST(Application, FirstFitDecreasingName) {
    FirstFitDecreasing ffd;
    EXPECT_EQ(ffd.name(), "FirstFitDecreasing");
}

TEST(Application, FirstFitDecreasingBasic) {
    FirstFitDecreasing ffd;

    Bin bin{"bin1", {10.0, 10.0, 10.0}, 100.0};
    std::vector<Item> items = {
        {"item1", {2.0, 2.0, 2.0}, 5.0},
        {"item2", {3.0, 3.0, 3.0}, 10.0},
        {"item3", {1.0, 1.0, 1.0}, 2.0}
    };

    auto solution = ffd.pack(bin, items);

    EXPECT_EQ(solution.bin_id, "bin1");
    EXPECT_EQ(solution.placed_items.size(), 3u);
    EXPECT_GT(solution.total_volume(), 0.0);
}

TEST(Application, FirstFitDecreasingEmpty) {
    FirstFitDecreasing ffd;
    Bin bin{"bin1", {10.0, 10.0, 10.0}, 100.0};
    std::vector<Item> items;

    auto solution = ffd.pack(bin, items);
    EXPECT_EQ(solution.placed_items.size(), 0u);
}

TEST(Application, LayerGenerator) {
    std::vector<PlacedItem> items;
    PlacedItem a{{"a", {2.0, 2.0, 2.0}}, {0.0, 0.0, 0.0}};
    PlacedItem b{{"b", {2.0, 2.0, 2.0}}, {2.0, 0.0, 0.0}};
    PlacedItem c{{"c", {2.0, 2.0, 2.0}}, {0.0, 0.0, 5.0}};
    items.push_back(a);
    items.push_back(b);
    items.push_back(c);

    auto layers = LayerGenerator::generate_layers(items);
    EXPECT_EQ(layers.size(), 2u);  // 两个不同的 z 坐标
}

// ============================================================================
// Context 测试 / Context tests
// ============================================================================

TEST(Bpp3dContext, Creation) {
    Bpp3dContext ctx;
    EXPECT_EQ(ctx.domain_name(), "BPP3D");
}

TEST(Bpp3dContext, AddItem) {
    Bpp3dContext ctx;
    ctx.add_item({"item1", {2.0, 3.0, 4.0}, 10.0});
    ctx.add_item({"item2", {1.0, 2.0, 3.0}, 5.0});

    EXPECT_EQ(ctx.items().size(), 2u);
}

TEST(Bpp3dContext, SetBin) {
    Bpp3dContext ctx;
    ctx.set_bin({"bin1", {10.0, 10.0, 10.0}, 100.0});

    EXPECT_TRUE(ctx.bin().has_value());
    EXPECT_EQ(ctx.bin()->id, "bin1");
}

TEST(Bpp3dContext, Solve) {
    Bpp3dContext ctx;
    ctx.set_bin({"bin1", {10.0, 10.0, 10.0}, 100.0});
    ctx.add_item({"item1", {2.0, 2.0, 2.0}, 5.0});
    ctx.add_item({"item2", {3.0, 3.0, 3.0}, 10.0});

    FirstFitDecreasing algorithm;
    auto solution = ctx.solve(algorithm);

    EXPECT_EQ(solution.bin_id, "bin1");
    EXPECT_EQ(solution.placed_items.size(), 2u);
}

TEST(Bpp3dContext, SolveWithoutBin) {
    Bpp3dContext ctx;
    ctx.add_item({"item1", {2.0, 2.0, 2.0}, 5.0});

    FirstFitDecreasing algorithm;
    auto solution = ctx.solve(algorithm);

    EXPECT_TRUE(solution.placed_items.empty());
}

// ============================================================================
// 综合测试 / Integration tests
// ============================================================================

TEST(Integration, Bpp3dWorkflow) {
    // 创建上下文
    Bpp3dContext ctx;

    // 设置容器
    ctx.set_bin({"standard_bin", {20.0, 20.0, 20.0}, 500.0});

    // 添加物品
    ctx.add_item({"box1", {5.0, 5.0, 5.0}, 20.0});
    ctx.add_item({"box2", {3.0, 3.0, 3.0}, 10.0});
    ctx.add_item({"box3", {2.0, 2.0, 2.0}, 5.0});
    ctx.add_item({"box4", {4.0, 4.0, 4.0}, 15.0});

    // 执行打包
    FirstFitDecreasing algorithm;
    auto solution = ctx.solve(algorithm);

    // 验证结果
    EXPECT_EQ(solution.placed_items.size(), 4u);
    EXPECT_GT(solution.total_volume(), 0.0);

    // 计算利用率
    double utilization = solution.utilization(ctx.bin()->volume());
    EXPECT_GT(utilization, 0.0);
    EXPECT_LE(utilization, 1.0);

    // 生成层
    auto layers = LayerGenerator::generate_layers(solution.placed_items);
    EXPECT_GT(layers.size(), 0u);
}
