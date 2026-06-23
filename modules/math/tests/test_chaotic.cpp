/// ospf_math chaotic/combinatorics/fractal 测试
/// 覆盖混沌系统、组合数学、分形。

#include <gtest/gtest.h>

#include <ospf/math/chaotic/lorenz.hpp>
#include <ospf/math/chaotic/rossler.hpp>
#include <ospf/math/chaotic/logistic.hpp>
#include <ospf/math/chaotic/henon.hpp>
#include <ospf/math/chaotic/chen.hpp>
#include <ospf/math/chaotic/maps.hpp>
#include <ospf/math/combinatorics.hpp>
#include <ospf/math/fractal.hpp>

#include <cmath>
#include <complex>
#include <numbers>

using namespace ospf::math;
using namespace ospf::math::chaotic;

// ============================================================================
// Chaotic systems 测试 / Chaotic systems tests
// ============================================================================

// Lorenz
TEST(Chaotic, LorenzStep) {
    LorenzState state = {1.0, 2.0, 3.0};
    auto next = lorenz_step(state, 0.01);
    // 验证状态发生变化（至少一个分量改变）
    EXPECT_TRUE(next[0] != state[0] || next[1] != state[1] || next[2] != state[2]);
}

TEST(Chaotic, LorenzIterate) {
    LorenzState state = {1.0, 1.0, 1.0};
    auto result = lorenz_iterate(state, 100, 0.01);
    // 迭代后状态应该在合理范围内（Lorenz 吸引子有界）
    EXPECT_TRUE(std::isfinite(result[0]));
    EXPECT_TRUE(std::isfinite(result[1]));
    EXPECT_TRUE(std::isfinite(result[2]));
}

// Rossler
TEST(Chaotic, RosslerStep) {
    RosslerState state = {1.0, 1.0, 1.0};
    auto next = rossler_step(state, 0.01);
    EXPECT_NE(next[0], state[0]);
}

TEST(Chaotic, RosslerIterate) {
    RosslerState state = {1.0, 1.0, 1.0};
    auto result = rossler_iterate(state, 100, 0.01);
    EXPECT_TRUE(std::isfinite(result[0]));
    EXPECT_TRUE(std::isfinite(result[1]));
    EXPECT_TRUE(std::isfinite(result[2]));
}

// Chen
TEST(Chaotic, ChenStep) {
    ChenState state = {1.0, 2.0, 3.0};
    auto next = chen_step(state, 0.01);
    EXPECT_TRUE(next[0] != state[0] || next[1] != state[1] || next[2] != state[2]);
}

TEST(Chaotic, ChenIterate) {
    ChenState state = {1.0, 2.0, 3.0};
    // Chen 系统需要较小的 dt 才能稳定
    auto result = chen_iterate(state, 100, 0.001);
    EXPECT_TRUE(std::isfinite(result[0]));
    EXPECT_TRUE(std::isfinite(result[1]));
    EXPECT_TRUE(std::isfinite(result[2]));
}

// Logistic map
TEST(Chaotic, LogisticMap) {
    // r=2.0: 固定点 x=0.5
    EXPECT_NEAR(logistic_iterate(0.1, 2.0, 100), 0.5, 1e-6);

    // r=3.2: 周期 2
    double x = logistic_iterate(0.1, 3.2, 1000);
    EXPECT_TRUE(std::isfinite(x));
}

TEST(Chaotic, LogisticMapBasic) {
    EXPECT_NEAR(logistic_map(0.5, 2.0), 0.5, 1e-10);  // 固定点
    EXPECT_NEAR(logistic_map(0.25, 4.0), 0.75, 1e-10); // r=4
}

// Henon map
TEST(Chaotic, HenonMap) {
    HenonState state = {0.1, 0.1};
    auto next = henon_map(state);
    EXPECT_NE(next[0], state[0]);
    EXPECT_TRUE(std::isfinite(next[0]));
    EXPECT_TRUE(std::isfinite(next[1]));
}

TEST(Chaotic, HenonIterate) {
    HenonState state = {0.1, 0.1};
    auto result = henon_iterate(state, 100);
    EXPECT_TRUE(std::isfinite(result[0]));
    EXPECT_TRUE(std::isfinite(result[1]));
}

// Maps
TEST(Chaotic, TentMap) {
    EXPECT_NEAR(tent_map(0.25), 0.5, 1e-10);
    EXPECT_NEAR(tent_map(0.75), 0.5, 1e-10);
    EXPECT_NEAR(tent_map(0.5), 1.0, 1e-10);
}

TEST(Chaotic, SineMap) {
    double x = sine_map(0.5);
    EXPECT_TRUE(std::isfinite(x));
    EXPECT_GT(x, 0.0);
}

TEST(Chaotic, DyadicMap) {
    EXPECT_NEAR(dyadic_map(0.25), 0.5, 1e-10);
    EXPECT_NEAR(dyadic_map(0.75), 0.5, 1e-10);
}

// ============================================================================
// Combinatorics 测试 / Combinatorics tests
// ============================================================================

TEST(Combinatorics, Factorial) {
    EXPECT_EQ(factorial(0), 1);
    EXPECT_EQ(factorial(1), 1);
    EXPECT_EQ(factorial(5), 120);
    EXPECT_EQ(factorial(10), 3628800);
}

TEST(Combinatorics, Permutations) {
    EXPECT_EQ(permutations(5, 3), 60);   // P(5,3) = 5*4*3 = 60
    EXPECT_EQ(permutations(10, 2), 90);  // P(10,2) = 10*9 = 90
}

TEST(Combinatorics, Combinations) {
    EXPECT_EQ(combinations(5, 3), 10);   // C(5,3) = 10
    EXPECT_EQ(combinations(10, 2), 45);  // C(10,2) = 45
    EXPECT_EQ(combinations(6, 0), 1);    // C(6,0) = 1
}

TEST(Combinatorics, AllPermutations) {
    std::vector<int> items = {1, 2, 3};
    auto perms = all_permutations(items);
    EXPECT_EQ(perms.size(), 6u);  // 3! = 6
}

TEST(Combinatorics, AllCombinations) {
    std::vector<int> items = {1, 2, 3, 4};
    auto combos = all_combinations(items, 2);
    EXPECT_EQ(combos.size(), 6u);  // C(4,2) = 6
}

TEST(Combinatorics, CartesianProduct) {
    std::vector<int> a = {1, 2};
    std::vector<int> b = {3, 4, 5};
    auto product = cartesian_product(a, b);
    EXPECT_EQ(product.size(), 6u);  // 2 * 3 = 6
}

TEST(Combinatorics, NewtonIterate) {
    // 求 x^2 - 2 = 0 的根（sqrt(2)）
    auto f = [](double x) { return x * x - 2.0; };
    auto df = [](double x) { return 2.0 * x; };
    double root = newton_iterate(f, df, 1.0);
    EXPECT_NEAR(root, std::sqrt(2.0), 1e-10);
}

// ============================================================================
// Fractal 测试 / Fractal tests
// ============================================================================

TEST(Fractal, MandelbrotInSet) {
    // 原点在 Mandelbrot 集内
    EXPECT_TRUE(is_in_mandelbrot({0.0, 0.0}));

    // (0.5, 0) 不在集合内
    EXPECT_FALSE(is_in_mandelbrot({0.5, 0.0}));

    // (-2, 0) 在边界附近
    EXPECT_TRUE(is_in_mandelbrot({-0.5, 0.0}));
}

TEST(Fractal, MandelbrotIterations) {
    // 原点：迭代次数 = max_iter（在集合内）
    EXPECT_EQ(mandelbrot_iterations({0.0, 0.0}, 100), 100u);

    // 远离原点：快速逃逸
    EXPECT_LT(mandelbrot_iterations({2.0, 2.0}, 100), 5u);
}

TEST(Fractal, JuliaInSet) {
    // 使用较小的 c 值，z=0 更可能在 Julia 集内
    std::complex<double> c(-0.1, 0.1);
    EXPECT_TRUE(is_in_julia({0.0, 0.0}, c, 100));
}

TEST(Fractal, JuliaIterations) {
    std::complex<double> c(-0.1, 0.1);
    auto iters = julia_iterations({0.0, 0.0}, c, 100);
    EXPECT_EQ(iters, 100u);  // 在集合内
}

TEST(Fractal, MandelbrotGrid) {
    auto grid = mandelbrot_grid(-2.0, 1.0, -1.0, 1.0, 10, 10, 50);
    EXPECT_EQ(grid.size(), 10u);
    EXPECT_EQ(grid[0].size(), 10u);

    // 左下角（-2, -1）应该在集合内或接近
    EXPECT_GE(grid[0][0], 1u);
}

// ============================================================================
// 综合测试 / Integration tests
// ============================================================================

TEST(Integration, CombinatoricsWorkflow) {
    // 计算 C(10, 5)
    auto c = combinations(10, 5);
    EXPECT_EQ(c, 252);

    // 生成所有排列
    std::vector<char> items = {'a', 'b', 'c'};
    auto perms = all_permutations(items);
    EXPECT_EQ(perms.size(), 6u);
}

TEST(Integration, FractalWorkflow) {
    // 检查 Mandelbrot 集的几个关键点
    EXPECT_TRUE(is_in_mandelbrot({-0.5, 0.0}));   // 主心脏线内
    EXPECT_FALSE(is_in_mandelbrot({2.0, 0.0}));    // 明显在集合外
    EXPECT_FALSE(is_in_mandelbrot({1.0, 1.0}));    // 在集合外
}
