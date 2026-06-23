/// BPP3D demo
/// 演示 3D 装箱问题的求解。

#include <ospf/framework/bpp3d/context.hpp>
#include <ospf/framework/bpp3d/application.hpp>
#include <iostream>

int main() {
    using namespace ospf::framework::bpp3d;

    std::cout << "=== BPP3D Demo ===" << std::endl;

    // 创建上下文
    Bpp3dContext ctx;

    // 设置容器（标准箱子 20x20x20）
    ctx.set_bin({"standard_bin", {20.0, 20.0, 20.0}, 500.0});

    // 添加物品
    ctx.add_item({"box1", {5.0, 5.0, 5.0}, 20.0});
    ctx.add_item({"box2", {3.0, 3.0, 3.0}, 10.0});
    ctx.add_item({"box3", {2.0, 2.0, 2.0}, 5.0});
    ctx.add_item({"box4", {4.0, 4.0, 4.0}, 15.0});
    ctx.add_item({"box5", {6.0, 6.0, 6.0}, 25.0});

    std::cout << "Items: " << ctx.items().size() << std::endl;
    std::cout << "Bin: " << ctx.bin()->id
              << " (" << ctx.bin()->dimensions.width << "x"
              << ctx.bin()->dimensions.height << "x"
              << ctx.bin()->dimensions.depth << ")" << std::endl;

    // 执行打包
    FirstFitDecreasing algorithm;
    auto solution = ctx.solve(algorithm);

    std::cout << "Algorithm: " << algorithm.name() << std::endl;
    std::cout << "Placed items: " << solution.placed_items.size() << std::endl;
    std::cout << "Total volume: " << solution.total_volume() << std::endl;

    double utilization = solution.utilization(ctx.bin()->volume());
    std::cout << "Utilization: " << (utilization * 100.0) << "%" << std::endl;

    // 生成层
    auto layers = LayerGenerator::generate_layers(solution.placed_items);
    std::cout << "Layers: " << layers.size() << std::endl;

    std::cout << "=== Demo Complete ===" << std::endl;
    return 0;
}
