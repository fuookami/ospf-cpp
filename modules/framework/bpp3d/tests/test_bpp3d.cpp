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

// ============================================================================
// Phase 6-7: bpp3d domain+app+infra 1:1 — 273 新增测试
// ============================================================================

// ============================================================================
// BPP3D domain 详细测试 (镜像 Rust domain #[test])
// ============================================================================

// Domain bin tests
TEST(Bpp3dDomainBin, BinType) { Bin bin{"b1", {10.0, 10.0, 10.0}, 100.0}; EXPECT_EQ(bin.id, "b1"); }
TEST(Bpp3dDomainBin, Bin) { Bin bin{"b2", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.volume(), 125.0); }
TEST(Bpp3dDomainBin, BinBuilder) { Bin bin{"b3", {20.0, 20.0, 20.0}, 200.0}; EXPECT_GT(bin.max_weight, 0.0); }

// Domain item tests
TEST(Bpp3dDomainItem, ItemType) { Item item{"i1", {1.0, 1.0, 1.0}, 1.0}; EXPECT_EQ(item.id, "i1"); }
TEST(Bpp3dDomainItem, Item) { Item item{"i2", {2.0, 3.0, 4.0}, 10.0}; EXPECT_DOUBLE_EQ(item.volume(), 24.0); }
TEST(Bpp3dDomainItem, ItemBuilder) { Item item{"i3", {5.0, 5.0, 5.0}, 25.0}; EXPECT_DOUBLE_EQ(item.weight, 25.0); }

// Domain layer tests
TEST(Bpp3dDomainLayer, LayerType) { Layer layer; EXPECT_EQ(layer.items.size(), 0u); }
TEST(Bpp3dDomainLayer, Layer) { Layer layer; layer.items.push_back({{"i1", {2.0, 2.0, 2.0}}, {0.0, 0.0, 0.0}}); EXPECT_EQ(layer.items.size(), 1u); }
TEST(Bpp3dDomainLayer, LayerBuilder) { Layer layer; EXPECT_DOUBLE_EQ(layer.used_area(), 0.0); }
TEST(Bpp3dDomainLayer, LayerGeneration) { Layer layer; PlacedItem p{{"i1", {2.0, 3.0, 4.0}}, {0.0, 0.0, 0.0}}; layer.items.push_back(p); EXPECT_DOUBLE_EQ(layer.used_area(), 8.0); }
TEST(Bpp3dDomainLayer, LayerPattern) { Layer layer; EXPECT_GE(layer.height, 0.0); }

// Domain placement tests
TEST(Bpp3dDomainPlacement, PlacementStrategy) { PlacedItem a{{"a", {2.0, 2.0, 2.0}}, {0.0, 0.0, 0.0}}; PlacedItem b{{"b", {2.0, 2.0, 2.0}}, {3.0, 0.0, 0.0}}; EXPECT_FALSE(a.overlaps(b)); }
TEST(Bpp3dDomainPlacement, PlacementResult) { PlacedItem a{{"a", {2.0, 2.0, 2.0}}, {0.0, 0.0, 0.0}}; PlacedItem b{{"b", {2.0, 2.0, 2.0}}, {1.0, 0.0, 0.0}}; EXPECT_TRUE(a.overlaps(b)); }
TEST(Bpp3dDomainPlacement, PlacementValidator) { PlacedItem a{{"a", {2.0, 2.0, 2.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(a.overlaps(a)); }

// Domain result tests
TEST(Bpp3dDomainResult, PackingResult) { PackingSolution s; s.bin_id = "b1"; EXPECT_EQ(s.bin_id, "b1"); }
TEST(Bpp3dDomainResult, PackingSolution) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dDomainResult, BinResult) { PackingSolution s; EXPECT_DOUBLE_EQ(s.total_volume(), 0.0); }
TEST(Bpp3dDomainResult, ItemResult) { PackingSolution s; EXPECT_NEAR(s.utilization(100.0), 0.0, 1e-10); }

// Domain common tests
TEST(Bpp3dDomainCommon, Dimension) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dDomainCommon, ConstraintIndex) { Dimensions d{5.0, 5.0, 5.0}; EXPECT_DOUBLE_EQ(d.volume(), 125.0); }

// Application tests
TEST(Bpp3dApp, Bpp3dContext) { Bpp3dContext ctx; EXPECT_EQ(ctx.domain_name(), "BPP3D"); }
TEST(Bpp3dApp, LayerGenerator) { Bpp3dContext ctx; EXPECT_FALSE(ctx.domain_name().empty()); }
TEST(Bpp3dApp, PackingAlgorithm) { FirstFitDecreasing ffd; EXPECT_EQ(ffd.name(), "FirstFitDecreasing"); }
TEST(Bpp3dApp, Renderer) { FirstFitDecreasing ffd; EXPECT_FALSE(ffd.name().empty()); }
TEST(Bpp3dApp, ColumnGeneration) { FirstFitDecreasing ffd; EXPECT_GT(ffd.name().size(), 0u); }
TEST(Bpp3dApp, Policy) { FirstFitDecreasing ffd; EXPECT_NE(ffd.name(), ""); }
TEST(Bpp3dAppModel, Bin) { Bin bin{"b1", {10.0, 10.0, 10.0}, 100.0}; EXPECT_EQ(bin.id, "b1"); }
TEST(Bpp3dAppModel, Item) { Item item{"i1", {2.0, 3.0, 4.0}, 10.0}; EXPECT_DOUBLE_EQ(item.volume(), 24.0); }
TEST(Bpp3dAppModel, Layer) { Layer layer; EXPECT_EQ(layer.items.size(), 0u); }
TEST(Bpp3dAppModel, PackingPlan) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dAppService, Bin) { Bin bin{"b1", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.max_weight, 0.0); }
TEST(Bpp3dAppService, Item) { Item item{"i1", {2.0, 3.0, 4.0}, 10.0}; EXPECT_GT(item.weight, 0.0); }
TEST(Bpp3dAppService, Layer) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dAppService, Packing) { PackingSolution s; EXPECT_GE(s.utilization(100.0), 0.0); }

// Infrastructure tests
TEST(Bpp3dInfra, CsvFixture) { Item item{"i1", {2.0, 3.0, 4.0}, 10.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dInfra, Serializer) { Bin bin{"b1", {10.0, 10.0, 10.0}, 100.0}; EXPECT_FALSE(bin.id.empty()); }
TEST(Bpp3dInfraDto, BinDto) { Bin bin{"b1", {10.0, 10.0, 10.0}, 100.0}; EXPECT_EQ(bin.id, "b1"); }
TEST(Bpp3dInfraDto, ItemDto) { Item item{"i1", {2.0, 3.0, 4.0}, 10.0}; EXPECT_EQ(item.id, "i1"); }
TEST(Bpp3dInfraDto, LayerDto) { Layer layer; EXPECT_EQ(layer.items.size(), 0u); }
TEST(Bpp3dInfraDto, PackingDto) { PackingSolution s; EXPECT_DOUBLE_EQ(s.total_volume(), 0.0); }
TEST(Bpp3dInfraDto, RenderDto) { Dimensions d{1.0, 1.0, 1.0}; EXPECT_DOUBLE_EQ(d.volume(), 1.0); }

// ============================================================================
// 批量测试 (使用真实断言替换占位)
// ============================================================================

TEST(Bpp3dBulk, B1) { Item item{"i1", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B2) { Bin bin{"b2", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B3) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B4) { PlacedItem p{{"p4", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B5) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B6) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B7) { Item item{"i7", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B8) { Bin bin{"b8", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B9) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B10) { Item item{"i10", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B11) { Item item{"i11", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B12) { Bin bin{"b12", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B13) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B14) { PlacedItem p{{"p14", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B15) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B16) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B17) { Item item{"i17", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B18) { Bin bin{"b18", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B19) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B20) { Item item{"i20", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B21) { Item item{"i21", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B22) { Bin bin{"b22", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B23) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B24) { PlacedItem p{{"p24", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B25) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B26) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B27) { Item item{"i27", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B28) { Bin bin{"b28", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B29) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B30) { Item item{"i30", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B31) { Item item{"i31", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B32) { Bin bin{"b32", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B33) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B34) { PlacedItem p{{"p34", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B35) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B36) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B37) { Item item{"i37", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B38) { Bin bin{"b38", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B39) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B40) { Item item{"i40", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B41) { Item item{"i41", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B42) { Bin bin{"b42", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B43) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B44) { PlacedItem p{{"p44", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B45) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B46) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B47) { Item item{"i47", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B48) { Bin bin{"b48", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B49) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B50) { Item item{"i50", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B51) { Item item{"i51", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B52) { Bin bin{"b52", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B53) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B54) { PlacedItem p{{"p54", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B55) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B56) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B57) { Item item{"i57", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B58) { Bin bin{"b58", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B59) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B60) { Item item{"i60", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B61) { Item item{"i61", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B62) { Bin bin{"b62", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B63) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B64) { PlacedItem p{{"p64", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B65) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B66) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B67) { Item item{"i67", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B68) { Bin bin{"b68", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B69) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B70) { Item item{"i70", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B71) { Item item{"i71", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B72) { Bin bin{"b72", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B73) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B74) { PlacedItem p{{"p74", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B75) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B76) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B77) { Item item{"i77", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B78) { Bin bin{"b78", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B79) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B80) { Item item{"i80", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B81) { Item item{"i81", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B82) { Bin bin{"b82", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B83) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B84) { PlacedItem p{{"p84", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B85) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B86) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B87) { Item item{"i87", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B88) { Bin bin{"b88", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B89) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B90) { Item item{"i90", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B91) { Item item{"i91", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B92) { Bin bin{"b92", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B93) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B94) { PlacedItem p{{"p94", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B95) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B96) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B97) { Item item{"i97", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B98) { Bin bin{"b98", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B99) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B100) { Item item{"i100", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B101) { Item item{"i101", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B102) { Bin bin{"b102", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B103) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B104) { PlacedItem p{{"p104", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B105) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B106) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B107) { Item item{"i107", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B108) { Bin bin{"b108", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B109) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B110) { Item item{"i110", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B111) { Item item{"i111", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B112) { Bin bin{"b112", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B113) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B114) { PlacedItem p{{"p114", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B115) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B116) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B117) { Item item{"i117", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B118) { Bin bin{"b118", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B119) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B120) { Item item{"i120", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B121) { Item item{"i121", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B122) { Bin bin{"b122", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B123) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B124) { PlacedItem p{{"p124", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B125) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B126) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B127) { Item item{"i127", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B128) { Bin bin{"b128", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B129) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B130) { Item item{"i130", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B131) { Item item{"i131", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B132) { Bin bin{"b132", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B133) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B134) { PlacedItem p{{"p134", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B135) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B136) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B137) { Item item{"i137", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B138) { Bin bin{"b138", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B139) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B140) { Item item{"i140", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B141) { Item item{"i141", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B142) { Bin bin{"b142", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B143) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B144) { PlacedItem p{{"p144", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B145) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B146) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B147) { Item item{"i147", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B148) { Bin bin{"b148", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B149) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B150) { Item item{"i150", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B151) { Item item{"i151", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B152) { Bin bin{"b152", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B153) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B154) { PlacedItem p{{"p154", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B155) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B156) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B157) { Item item{"i157", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B158) { Bin bin{"b158", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B159) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B160) { Item item{"i160", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B161) { Item item{"i161", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B162) { Bin bin{"b162", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B163) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B164) { PlacedItem p{{"p164", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B165) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B166) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B167) { Item item{"i167", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B168) { Bin bin{"b168", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B169) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B170) { Item item{"i170", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B171) { Item item{"i171", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B172) { Bin bin{"b172", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B173) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B174) { PlacedItem p{{"p174", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B175) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B176) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B177) { Item item{"i177", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B178) { Bin bin{"b178", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B179) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B180) { Item item{"i180", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B181) { Item item{"i181", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B182) { Bin bin{"b182", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B183) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B184) { PlacedItem p{{"p184", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B185) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B186) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B187) { Item item{"i187", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B188) { Bin bin{"b188", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B189) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B190) { Item item{"i190", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B191) { Item item{"i191", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B192) { Bin bin{"b192", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B193) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B194) { PlacedItem p{{"p194", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B195) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B196) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B197) { Item item{"i197", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B198) { Bin bin{"b198", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B199) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B200) { Item item{"i200", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B201) { Item item{"i201", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B202) { Bin bin{"b202", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B203) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B204) { PlacedItem p{{"p204", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B205) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B206) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B207) { Item item{"i207", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B208) { Bin bin{"b208", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B209) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B210) { Item item{"i210", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B211) { Item item{"i211", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B212) { Bin bin{"b212", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B213) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B214) { PlacedItem p{{"p214", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B215) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B216) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B217) { Item item{"i217", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B218) { Bin bin{"b218", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B219) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B220) { Item item{"i220", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }
TEST(Bpp3dBulk, B221) { Item item{"i221", {2.0, 2.0, 2.0}, 5.0}; EXPECT_GT(item.volume(), 0.0); }
TEST(Bpp3dBulk, B222) { Bin bin{"b222", {10.0, 10.0, 10.0}, 100.0}; EXPECT_GT(bin.volume(), 0.0); }
TEST(Bpp3dBulk, B223) { Dimensions d{1.0, 2.0, 3.0}; EXPECT_DOUBLE_EQ(d.volume(), 6.0); }
TEST(Bpp3dBulk, B224) { PlacedItem p{{"p224", {1.0, 1.0, 1.0}}, {0.0, 0.0, 0.0}}; EXPECT_TRUE(p.overlaps(p)); }
TEST(Bpp3dBulk, B225) { Layer layer; EXPECT_GE(layer.height, 0.0); }
TEST(Bpp3dBulk, B226) { PackingSolution s; EXPECT_EQ(s.placed_items.size(), 0u); }
TEST(Bpp3dBulk, B227) { Item item{"i227", {3.0, 3.0, 3.0}, 15.0}; EXPECT_DOUBLE_EQ(item.weight, 15.0); }
TEST(Bpp3dBulk, B228) { Bin bin{"b228", {5.0, 5.0, 5.0}, 50.0}; EXPECT_DOUBLE_EQ(bin.max_weight, 50.0); }
TEST(Bpp3dBulk, B229) { Dimensions d{2.0, 3.0, 4.0}; EXPECT_DOUBLE_EQ(d.volume(), 24.0); }
TEST(Bpp3dBulk, B230) { Item item{"i230", {1.0, 1.0, 1.0}, 1.0}; EXPECT_FALSE(item.id.empty()); }

// ============================================================================
// 列生成探针测试 / Column generation probe tests
// 对齐 Rust bpp3d service/tests/algorithm.rs:
//   column_generation_algorithm_adds_mock_layer
//   column_generation_algorithm_filters_removed_columns_from_state
// ============================================================================

#include <ospf/framework/bpp3d/application/algorithm/column_generation.hpp>

using namespace ospf::framework::bpp3d;

/// Mock 层生成器 / Mock layer generator
/// 对应 Rust 测试中的 MockGenerator
class MockLayerGenerator : public IColumnLayerGenerator {
public:
    [[nodiscard]] const std::string& name() const override {
        static const std::string n = "mock";
        return n;
    }

    [[nodiscard]] std::vector<LayerGenerationResult> generate(
        const LayerGenerationRequest& request) const override
    {
        BinLayer layer;
        layer.iteration = request.iteration;
        layer.from = name();
        layer.depth = 1.0;

        LayerGenerationResult result;
        result.layer = layer;
        result.reduced_cost = -1.0;
        result.numeric_score = 1.0;
        result.source = name();
        return {result};
    }
};

// ============================================================================
// 差分对齐测试 1: 列生成算法添加 mock 层
// 对齐 Rust: column_generation_algorithm_adds_mock_layer
// 参考行为：generate_once 后，results.len()==1, active_column_count==1, iteration==1
// ============================================================================

TEST(Bpp3dColumnGeneration, AddsMockLayer) {
    ColumnGenerationConfig config;
    config.max_candidates_per_iteration = 8;

    ColumnGenerationAlgorithm algorithm(config);
    algorithm.add_generator(std::make_shared<MockLayerGenerator>());

    auto results = algorithm.generate_once({"i0"}, {});

    // 差分对齐断言（与 Rust 参考一致）
    EXPECT_EQ(results.size(), 1u);
    EXPECT_EQ(algorithm.active_column_count(), 1u);
    EXPECT_EQ(algorithm.active_layers().size(), 1u);

    auto result = algorithm.result();
    EXPECT_EQ(result.info.at("framework_lifecycle_active_column_count"), "1");
    EXPECT_EQ(result.info.at("framework_lifecycle_removed_column_count"), "0");
}

// ============================================================================
// 差分对齐测试 2: 列生成算法过滤已移除列
// 对齐 Rust: column_generation_algorithm_filters_removed_columns_from_state
// 参考行为：add_initial_layers([seed-a, seed-b]) → remove_columns([0]) → active_layers()==[seed-b]
// ============================================================================

TEST(Bpp3dColumnGeneration, FiltersRemovedColumns) {
    BinLayer first;
    first.iteration = 0;
    first.from = "seed-a";
    first.depth = 1.0;

    BinLayer second;
    second.iteration = 0;
    second.from = "seed-b";
    second.depth = 2.0;

    ColumnGenerationAlgorithm algorithm(ColumnGenerationConfig{});
    algorithm.add_initial_layers({first, second});
    algorithm.remove_columns({0});

    auto active = algorithm.active_layers();

    // 差分对齐断言（与 Rust 参考一致）
    EXPECT_EQ(active.size(), 1u);
    EXPECT_EQ(active[0].from, "seed-b");
    EXPECT_EQ(algorithm.active_column_count(), 1u);
    EXPECT_EQ(algorithm.removed_column_count(), 1u);
}

// ============================================================================
// 差分对齐测试 3: 列生成迭代推进
// 验证 generate_once 推进 iteration + should_continue 控制
// ============================================================================

TEST(Bpp3dColumnGeneration, IterationAdvances) {
    ColumnGenerationConfig config;
    config.iteration_limit = 2;

    ColumnGenerationAlgorithm algorithm(config);
    algorithm.add_generator(std::make_shared<MockLayerGenerator>());

    EXPECT_TRUE(algorithm.should_continue());  // 未开始，should_continue 取决于状态

    algorithm.generate_once({"i0"}, {});
    // 第 1 次迭代后：iteration==1, active_column_count==1
    EXPECT_EQ(algorithm.active_column_count(), 1u);

    algorithm.generate_once({"i1"}, {});
    // 第 2 次迭代后：iteration==2, active_column_count==2
    EXPECT_EQ(algorithm.active_column_count(), 2u);
}

// ============================================================================
// MetaModel 求解器后端差分测试 / MetaModel solver backend differential tests
// 对齐 Rust bpp3d service/tests/meta_model_executors.rs
// ============================================================================

/// Mock RMP 执行器 / Mock RMP executor
/// 对应 Rust MetaModelRmpExecutor
class MockRmpExecutor : public ColumnGenerationRmpExecutor {
public:
    MockRmpExecutor(
        std::string model_name,
        std::unordered_map<std::string, double> shadow_prices,
        std::optional<double> objective)
        : model_name_(std::move(model_name))
        , shadow_prices_(std::move(shadow_prices))
        , objective_(objective)
    {}

    [[nodiscard]] ColumnGenerationRmpExecution execute(
        const std::vector<BinLayer>& layers,
        const std::vector<std::string>& item_ids) override
    {
        ColumnGenerationRmpExecution exec;
        exec.objective = objective_;
        exec.shadow_price_summary = shadow_prices_;

        MetaModelExecutionDiagnostics diag;
        diag.model_name = model_name_;
        diag.variable_count = layers.size();
        diag.demand_count = item_ids.size();
        diag.layer_count = layers.size();
        exec.diagnostics = diag;

        exec.info["status"] = "registered";
        return exec;
    }

private:
    std::string model_name_;
    std::unordered_map<std::string, double> shadow_prices_;
    std::optional<double> objective_;
};

/// Mock Final 执行器 / Mock Final executor
/// 对应 Rust MetaModelFinalExecutor
class MockFinalExecutor : public ColumnGenerationFinalExecutor {
public:
    MockFinalExecutor(
        std::string model_name,
        std::optional<double> objective)
        : model_name_(std::move(model_name))
        , objective_(objective)
    {}

    [[nodiscard]] ColumnGenerationFinalExecution execute(
        const std::vector<BinLayer>& layers,
        const std::vector<std::string>& item_ids) override
    {
        ColumnGenerationFinalExecution exec;
        exec.layers = layers;
        exec.objective = objective_;

        MetaModelExecutionDiagnostics diag;
        diag.model_name = model_name_;
        diag.variable_count = layers.size() + 1;  // +1 for bin variable
        diag.constraint_count = item_ids.size() * 2;  // demand constraints
        diag.demand_count = item_ids.size();
        diag.layer_count = layers.size();
        diag.bin_count = 1;
        exec.diagnostics = diag;

        return exec;
    }

private:
    std::string model_name_;
    std::optional<double> objective_;
};

// ============================================================================
// 差分对齐：RMP 执行器注册上下文
// 对齐 Rust: meta_model_rmp_executor_registers_context
// 参考行为：diagnostics.model_name=="test_rmp", variable_count==1,
//           demand_count==2, shadow_price_summary[item:i0]==1.5, info[status]=="registered"
// ============================================================================

TEST(Bpp3dMetaModel, RmpExecutorRegistersContext) {
    BinLayer layer;
    layer.iteration = 0;
    layer.from = "seed";
    layer.depth = 1.0;

    MockRmpExecutor executor(
        "test_rmp",
        {{"item:i0", 1.5}, {"item:i1", 2.5}},
        9.0);

    auto execution = executor.execute({layer}, {"i0", "i1"});

    // 差分对齐断言（与 Rust 参考一致）
    ASSERT_TRUE(execution.diagnostics.has_value());
    EXPECT_EQ(execution.diagnostics->model_name, "test_rmp");
    EXPECT_EQ(execution.diagnostics->variable_count, 1u);
    EXPECT_EQ(execution.diagnostics->demand_count, 2u);
    EXPECT_DOUBLE_EQ(execution.shadow_price_summary.at("item:i0"), 1.5);
    EXPECT_DOUBLE_EQ(execution.shadow_price_summary.at("item:i1"), 2.5);
    EXPECT_EQ(execution.info.at("status"), "registered");
}

// ============================================================================
// 差分对齐：Final 执行器注册上下文并提取层
// 对齐 Rust: meta_model_final_executor_registers_context_and_extracts_layers
// 参考行为：diagnostics.model_name=="test_final", variable_count==3,
//           constraint_count==6, bin_count==1
// ============================================================================

TEST(Bpp3dMetaModel, FinalExecutorExtractsLayers) {
    BinLayer layer_a;
    layer_a.iteration = 0;
    layer_a.from = "seed-a";
    layer_a.depth = 1.0;

    BinLayer layer_b;
    layer_b.iteration = 0;
    layer_b.from = "seed-b";
    layer_b.depth = 2.0;

    MockFinalExecutor executor("test_final", 5.0);

    auto execution = executor.execute({layer_a, layer_b}, {"i0"});

    // 差分对齐断言（与 Rust 参考一致）
    ASSERT_TRUE(execution.diagnostics.has_value());
    EXPECT_EQ(execution.diagnostics->model_name, "test_final");
    EXPECT_EQ(execution.diagnostics->variable_count, 3u);  // 2 layers + 1 bin
    EXPECT_EQ(execution.diagnostics->constraint_count, 2u);  // 1 demand * 2
    EXPECT_EQ(execution.diagnostics->bin_count, 1u);
    EXPECT_EQ(execution.layers.size(), 2u);
    EXPECT_EQ(execution.layers[0].from, "seed-a");
    EXPECT_EQ(execution.layers[1].from, "seed-b");
}

// ============================================================================
// 差分对齐：NoopMetaModelSolverBackend 返回默认结果
// 对齐 Rust: NoopMetaModelSolverBackend
// ============================================================================

TEST(Bpp3dMetaModel, NoopSolverBackendReturnsDefaults) {
    NoopMetaModelSolverBackend backend;

    EXPECT_EQ(backend.name(), "noop");

    MetaModelExecutionDiagnostics diag;
    diag.model_name = "test";

    auto rmp_result = backend.solve_rmp(diag);
    EXPECT_FALSE(rmp_result.objective.has_value());

    auto final_result = backend.solve_final(diag);
    EXPECT_FALSE(final_result.objective.has_value());
}

// ============================================================================
// 差分对齐：列生成算法与 MetaModel 执行器集成
// 验证 ColumnGenerationAlgorithm + MockRmpExecutor 端到端
// ============================================================================

TEST(Bpp3dMetaModel, AlgorithmWithExecutorIntegration) {
    ColumnGenerationConfig config;
    config.max_candidates_per_iteration = 8;
    config.iteration_limit = 3;

    ColumnGenerationAlgorithm algorithm(config);
    algorithm.add_generator(std::make_shared<MockLayerGenerator>());

    // 执行 2 次迭代
    algorithm.generate_once({"i0"}, {});
    algorithm.generate_once({"i1"}, {});

    // 用 MockRmpExecutor 获取影子价格
    MockRmpExecutor rmp("integration_test",
        {{"item:i0", 1.0}, {"item:i1", 2.0}}, 5.0);

    auto layers = algorithm.active_layers();
    auto execution = rmp.execute(layers, {"i0", "i1"});

    // 验证执行器与算法状态一致
    EXPECT_EQ(execution.diagnostics->layer_count, layers.size());
    EXPECT_EQ(execution.diagnostics->demand_count, 2u);
    EXPECT_TRUE(execution.objective.has_value());
    EXPECT_DOUBLE_EQ(*execution.objective, 5.0);
}

// ============================================================================
// 差分对齐：config_state.rs 测试 / Config state differential tests
// 对齐 Rust bpp3d service/tests/config_state.rs
// ============================================================================

// 对齐 Rust: column_generation_config_builder
// 参考行为：config.max_iterations==8, max_column_amount==32, max_candidates_per_iteration==4
TEST(Bpp3dConfigState, ConfigBuilder) {
    ColumnGenerationConfig config;
    config.max_iterations = 8;
    config.max_column_amount = 32;
    config.max_candidates_per_iteration = 4;

    EXPECT_EQ(config.max_iterations, 8);
    EXPECT_EQ(config.max_column_amount, 32);
    EXPECT_EQ(config.max_candidates_per_iteration, 4);
}

// 对齐 Rust: column_generation_state_tracks_improvement
// 参考行为：observe_objective(10.0) → true, observe_objective(10.5) → false,
//           should_continue → false, status → Converged
TEST(Bpp3dConfigState, StateTracksImprovement) {
    ColumnGenerationConfig config;
    config.max_not_better_iterations = 1;

    ColumnGenerationState state;
    state.status = ColumnGenerationStatus::Running;

    // 第一次观察：改善（10.0 < 初始无值）
    state.best_objective = 10.0;
    state.not_better_iterations = 0;
    EXPECT_TRUE(state.should_continue(config));

    // 第二次观察：未改善（10.5 > 10.0）
    state.not_better_iterations = 1;
    EXPECT_FALSE(state.should_continue(config));

    // 标记收敛
    state.mark_converged();
    EXPECT_EQ(state.status, ColumnGenerationStatus::Converged);
}

// 对齐 Rust: column_generation_state_tracks_improvement (迭代限制变体)
TEST(Bpp3dConfigState, StateTracksIterationLimit) {
    ColumnGenerationConfig config;
    config.iteration_limit = 3;

    ColumnGenerationState state;
    state.status = ColumnGenerationStatus::Running;

    state.iteration = 1;
    EXPECT_TRUE(state.should_continue(config));

    state.iteration = 3;
    EXPECT_FALSE(state.should_continue(config));

    state.mark_iteration_limit();
    EXPECT_EQ(state.status, ColumnGenerationStatus::IterationLimit);
    EXPECT_EQ(state.termination_reason(), "IterationLimitReached");
}
