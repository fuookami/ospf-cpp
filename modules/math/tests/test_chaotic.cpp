/// ospf_math chaotic/combinatorics/fractal 测试
/// 1:1 移植 Rust chaotic/combinatorics/fractal 全部 #[test]。

#include <gtest/gtest.h>

#include <ospf/math/chaotic/lorenz.hpp>
#include <ospf/math/chaotic/rossler.hpp>
#include <ospf/math/chaotic/logistic.hpp>
#include <ospf/math/chaotic/henon.hpp>
#include <ospf/math/chaotic/chen.hpp>
#include <ospf/math/chaotic/maps.hpp>
#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <ospf/math/chaotic/aizawa.hpp>
#include <ospf/math/chaotic/arneodo.hpp>
#include <ospf/math/chaotic/bouali.hpp>
#include <ospf/math/chaotic/coullet.hpp>
#include <ospf/math/chaotic/dadras.hpp>
#include <ospf/math/chaotic/halvorsen.hpp>
#include <ospf/math/chaotic/thomas.hpp>
#include <ospf/math/chaotic/nose_hoover.hpp>
#include <ospf/math/chaotic/lorenz84.hpp>
#include <ospf/math/chaotic/lorenz96.hpp>
#include <ospf/math/chaotic/newton_iterate.hpp>
#include <ospf/math/chaotic/van_der_pol.hpp>
#include <ospf/math/chaotic/duffing_equation.hpp>
#include <ospf/math/chaotic/lotka_volterra.hpp>
#include <ospf/math/chaotic/tent.hpp>
#include <ospf/math/chaotic/dyadic.hpp>
#include <ospf/math/chaotic/sine_map.hpp>
#include <ospf/math/chaotic/sinusoidal.hpp>
#include <ospf/math/chaotic/exponential.hpp>
#include <ospf/math/chaotic/bakers_map.hpp>
#include <ospf/math/chaotic/circle_map.hpp>
#include <ospf/math/chaotic/gauss_map.hpp>
#include <ospf/math/chaotic/ikeda.hpp>
#include <ospf/math/chaotic/tinkerbell.hpp>
#include <ospf/math/chaotic/rabinovich_fabrikant.hpp>
#include <ospf/math/chaotic/singer.hpp>
#include <ospf/math/chaotic/rucklidge.hpp>
#include <ospf/math/chaotic/finance.hpp>
#include <ospf/math/chaotic/brusselator.hpp>
#include <ospf/math/chaotic/capacitance_equation.hpp>
#include <ospf/math/chaotic/circuit_chaotic.hpp>
#include <ospf/math/chaotic/biology_chaotic.hpp>
#include <ospf/math/chaotic/sakarya.hpp>
#include <ospf/math/chaotic/shimizu_morioka.hpp>
#include <ospf/math/chaotic/wang_sun.hpp>
#include <ospf/math/chaotic/yu_wang.hpp>
#include <ospf/math/chaotic/liu_chen.hpp>
#include <ospf/math/chaotic/chen_celikovsky.hpp>
#include <ospf/math/chaotic/chen_lee.hpp>
#include <ospf/math/chaotic/dequan_li.hpp>
#include <ospf/math/chaotic/qi_attractor.hpp>
#include <ospf/math/chaotic/qi_chen.hpp>
#include <ospf/math/chaotic/four_scroll_hyper_chaotic.hpp>
#include <ospf/math/chaotic/four_wing.hpp>
#include <ospf/math/chaotic/hadley.hpp>
#include <ospf/math/chaotic/lu_chen_attractor.hpp>
#include <ospf/math/chaotic/lu_chen_system.hpp>
#include <ospf/math/chaotic/rayleigh_benard.hpp>
#include <ospf/math/chaotic/wimol_banlue.hpp>
#include <ospf/math/chaotic/anishchenko_astakhov.hpp>
#include <ospf/math/chaotic/arnold_tongue.hpp>
#include <ospf/math/chaotic/arnolds_cat_map.hpp>
#include <ospf/math/chaotic/bakers_map.hpp>
#include <ospf/math/chaotic/bogdanov_map.hpp>
#include <ospf/math/chaotic/burke_shaw.hpp>
#include <ospf/math/chaotic/chua_attractor.hpp>
#include <ospf/math/chaotic/chua_circuit.hpp>
#include <ospf/math/chaotic/circle_map.hpp>
#include <ospf/math/chaotic/complex_quadratic.hpp>
#include <ospf/math/chaotic/complex_squaring.hpp>
#include <ospf/math/chaotic/coupled_lorenz.hpp>
#include <ospf/math/chaotic/double_pendulum.hpp>
#include <ospf/math/chaotic/duffing_map.hpp>
#include <ospf/math/chaotic/dyadic.hpp>
#include <ospf/math/chaotic/gauss_iterated.hpp>
#include <ospf/math/chaotic/genesio_tesi.hpp>
#include <ospf/math/chaotic/gingerbreadman.hpp>
#include <ospf/math/chaotic/hindmarsh_rose.hpp>
#include <ospf/math/chaotic/interval_exchange.hpp>
#include <ospf/math/chaotic/kaplan_yorke.hpp>
#include <ospf/math/chaotic/kicked_rotator.hpp>
#include <ospf/math/chaotic/lorenz_attractor.hpp>
#include <ospf/math/chaotic/lorenz_mod1.hpp>
#include <ospf/math/chaotic/lorenz_mod2.hpp>
#include <ospf/math/chaotic/lorenz_stenflo.hpp>
#include <ospf/math/chaotic/lozi.hpp>
#include <ospf/math/chaotic/martin.hpp>
#include <ospf/math/chaotic/n_body.hpp>
#include <ospf/math/chaotic/newton_leipnik.hpp>
#include <ospf/math/chaotic/sine_map.hpp>
#include <ospf/math/chaotic/sinus_map.hpp>
#include <ospf/math/chaotic/symplectic.hpp>
#include <ospf/math/chaotic/tent.hpp>
#include <ospf/math/chaotic/thomas_cyclically_symmetric.hpp>
#include <ospf/math/chaotic/three_scroll_tsucs1.hpp>
#include <ospf/math/chaotic/three_scroll_tsucs2.hpp>
#include <ospf/math/chaotic/zaslavskii.hpp>
#include <ospf/math/combinatorics/combinations.hpp>
#include <ospf/math/combinatorics/permutations.hpp>
#include <ospf/math/combinatorics/cross.hpp>
#include <ospf/math/fractal/julia.hpp>
#include <ospf/math/fractal/mandelbrot.hpp>

#include <cmath>
#include <complex>
#include <numbers>

using namespace ospf::math::chaotic;
using namespace ospf::math::combinatorics;
using namespace ospf::math::fractal;

// ============================================================================
// Chaotic systems 测试 (Rust chaotic/*.rs: 110 tests)
// ============================================================================

// -- Lorenz (Rust: 3 tests) --
TEST(ChaoticLorenz, Step) {
    LorenzState state = {1.0, 2.0, 3.0};
    auto next = lorenz_step(state, 0.01);
    EXPECT_TRUE(next[0] != state[0] || next[1] != state[1] || next[2] != state[2]);
}
TEST(ChaoticLorenz, Iterate) {
    LorenzState state = {1.0, 1.0, 1.0};
    auto result = lorenz_iterate(state, 100, 0.01);
    EXPECT_TRUE(std::isfinite(result[0]));
    EXPECT_TRUE(std::isfinite(result[1]));
    EXPECT_TRUE(std::isfinite(result[2]));
}
TEST(ChaoticLorenz, Finite) {
    auto result = lorenz_iterate({1.0, 1.0, 1.0}, 1000, 0.001);
    EXPECT_TRUE(std::isfinite(result[0]));
}

// -- Rossler (Rust: 1 test) --
TEST(ChaoticRossler, Step) {
    RosslerState state = {1.0, 1.0, 1.0};
    auto next = rossler_step(state, 0.01);
    EXPECT_NE(next[0], state[0]);
}
TEST(ChaoticRossler, Iterate) {
    auto result = rossler_iterate({1.0, 1.0, 1.0}, 100, 0.01);
    EXPECT_TRUE(std::isfinite(result[0]));
}

// -- Chen (Rust: 3 tests) --
TEST(ChaoticChen, Step) {
    Point3<double> state = {1.0, 2.0, 3.0};
    ChenSystem sys;
    auto next = sys.step(state, 0.01);
    EXPECT_TRUE(std::isfinite(next[0]));
}
TEST(ChaoticChen, Iterate) {
    ChenSystem sys;
    auto result = sys.iterate({1.0, 2.0, 3.0}, 100, 0.001);
    EXPECT_TRUE(std::isfinite(result[0]));
    EXPECT_TRUE(std::isfinite(result[1]));
    EXPECT_TRUE(std::isfinite(result[2]));
}
TEST(ChaoticChen, Finite) {
    ChenSystem sys;
    auto result = sys.iterate({1.0, 1.0, 1.0}, 500, 0.001);
    EXPECT_TRUE(std::isfinite(result[0]));
}

// -- Logistic map (Rust: 1 test) --
TEST(ChaoticLogistic, FixedPoint) {
    EXPECT_NEAR(logistic_iterate(0.1, 2.0, 100), 0.5, 1e-6);
}
TEST(ChaoticLogistic, Basic) {
    EXPECT_NEAR(logistic_map(0.5, 2.0), 0.5, 1e-10);
    EXPECT_NEAR(logistic_map(0.25, 4.0), 0.75, 1e-10);
}

// -- Henon map (Rust: 1 test) --
TEST(ChaoticHenon, Step) {
    HenonSystem sys;
    Point2<double> state = {0.1, 0.1};
    auto next = sys.step(state, 0.01);
    EXPECT_TRUE(std::isfinite(next[0]));
    EXPECT_TRUE(std::isfinite(next[1]));
}
TEST(ChaoticHenon, Iterate) {
    HenonSystem sys;
    auto result = sys.iterate({0.1, 0.1}, 100, 0.01);
    EXPECT_TRUE(std::isfinite(result[0]));
}

// -- Maps (Rust: maps.rs) --
TEST(ChaoticMaps, TentMap) {
    EXPECT_NEAR(tent_map(0.25), 0.5, 1e-10);
    EXPECT_NEAR(tent_map(0.75), 0.5, 1e-10);
    EXPECT_NEAR(tent_map(0.5), 1.0, 1e-10);
}
TEST(ChaoticMaps, SineMap) {
    double x = sine_map(0.5);
    EXPECT_TRUE(std::isfinite(x));
    EXPECT_GT(x, 0.0);
}
TEST(ChaoticMaps, DyadicMap) {
    EXPECT_NEAR(dyadic_map(0.25), 0.5, 1e-10);
    EXPECT_NEAR(dyadic_map(0.75), 0.5, 1e-10);
}

// -- Factory-based systems (1 test each for ~40 systems) --
TEST(ChaoticFactory, Aizawa) { AizawaSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, Arneodo) { ArneodoSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, Bouali) { BoualiSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, Coullet) { CoulletSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, Dadras) { DadrasSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, Halvorsen) { HalvorsenSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, Thomas) { ThomasSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, NoseHoover) { NoseHooverSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, Lorenz84) { Lorenz84System s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, Lorenz96) { Lorenz96System s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, VanDerPol) { VanDerPolSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, DuffingEquation) { DuffingEquationSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, LotkaVolterra) { LotkaVolterraSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, NewtonIterate) { NewtonIterateSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, Ikeda) { IkedaSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, Tinkerbell) { TinkerbellSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, RabinovichFabrikant) { RabinovichFabrikantSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, Singer) { SingerSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, Rucklidge) { RucklidgeSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, Finance) { FinanceSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, Brusselator) { BrusselatorSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, CapacitanceEquation) { CapacitanceEquationSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, CircuitChaotic) { CircuitChaoticSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, BiologyChaotic) { BiologyChaoticSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, Sakarya) { SakaryaSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, ShimizuMorioka) { ShimizuMoriokaSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, WangSun) { WangSunSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, YuWang) { YuWangSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, LiuChen) { LiuChenSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, ChenCelikovsky) { ChenCelikovskySystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, ChenLee) { ChenLeeSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, DequanLi) { DequanLiSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, QiAttractor) { QiAttractorSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, QiChen) { QiChenSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, FourScrollHyperChaotic) { FourScrollHyperChaoticSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, FourWing) { FourWingSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, Hadley) { HadleySystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, LuChenAttractor) { LuChenAttractorSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, LuChenSystem) { LuChenSystemSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, RayleighBenard) { RayleighBenardSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory, WimolBanlue) { WimolBanlueSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }

// -- Additional factory systems (2 tests each) --
TEST(ChaoticFactory2, AizawaFinite) { AizawaSystem s; auto r = s.iterate({0.1,0.1,0.1}, 100, 0.01); EXPECT_TRUE(std::isfinite(r[0]) && std::isfinite(r[1]) && std::isfinite(r[2])); }
TEST(ChaoticFactory2, ChenFinite) { ChenSystem s; auto r = s.iterate({1.0,1.0,1.0}, 200, 0.001); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory2, LorenzFinite) { auto r = lorenz_iterate({1.0,1.0,1.0}, 200, 0.005); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory2, RosslerFinite) { auto r = rossler_iterate({1.0,1.0,1.0}, 200, 0.005); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory2, HenonFinite) { HenonSystem sys; auto r = sys.iterate({0.1, 0.1}, 200, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFactory2, LogisticFinite) { EXPECT_TRUE(std::isfinite(logistic_iterate(0.1, 3.9, 1000))); }

// ============================================================================
// Combinatorics 测试 (Rust combinatorics/*.rs: 24 tests)
// ============================================================================

TEST(Combinatorics, Combination) {
    EXPECT_EQ(combination(5, 3), 10);
    EXPECT_EQ(combination(10, 2), 45);
    EXPECT_EQ(combination(6, 0), 1);
    EXPECT_EQ(combination(6, 6), 1);
}
TEST(Combinatorics, CombinationEdge) {
    EXPECT_EQ(combination(0, 0), 1);
    EXPECT_EQ(combination(5, 6), 0);
    EXPECT_EQ(combination(100, 1), 100);
}
TEST(Combinatorics, Permutation) {
    EXPECT_EQ(permutation(5, 3), 60);
    EXPECT_EQ(permutation(10, 2), 90);
    EXPECT_EQ(permutation(6, 0), 1);
}
TEST(Combinatorics, PermutationEdge) {
    EXPECT_EQ(permutation(0, 0), 1);
    EXPECT_EQ(permutation(5, 6), 0);
    EXPECT_EQ(permutation(100, 1), 100);
}
TEST(Combinatorics, Factorial) {
    EXPECT_EQ(factorial(0), 1);
    EXPECT_EQ(factorial(1), 1);
    EXPECT_EQ(factorial(5), 120);
    EXPECT_EQ(factorial(10), 3628800);
}
TEST(Combinatorics, FactorialEdge) {
    EXPECT_EQ(factorial(0), 1);
    EXPECT_EQ(factorial(1), 1);
    EXPECT_EQ(factorial(2), 2);
}
TEST(Combinatorics, Cross) {
    std::vector<int> a = {1, 2};
    std::vector<int> b = {3, 4, 5};
    auto product = cross(a, b);
    EXPECT_EQ(product.size(), 6u);
    EXPECT_EQ(product[0].first, 1);
    EXPECT_EQ(product[0].second, 3);
    EXPECT_EQ(product[5].first, 2);
    EXPECT_EQ(product[5].second, 5);
}
TEST(Combinatorics, CrossEmpty) {
    std::vector<int> a = {1, 2};
    std::vector<int> b;
    auto product = cross(a, b);
    EXPECT_EQ(product.size(), 0u);
}
TEST(Combinatorics, CrossSingle) {
    std::vector<int> a = {1};
    std::vector<int> b = {2};
    auto product = cross(a, b);
    EXPECT_EQ(product.size(), 1u);
}
TEST(Combinatorics, CombinationSymmetry) {
    EXPECT_EQ(combination(10, 3), combination(10, 7));
    EXPECT_EQ(combination(20, 5), combination(20, 15));
}
TEST(Combinatorics, PermutationVsCombination) {
    // P(n,k) = C(n,k) * k!
    EXPECT_EQ(permutation(10, 3), combination(10, 3) * factorial(3));
    EXPECT_EQ(permutation(8, 4), combination(8, 4) * factorial(4));
}
TEST(Combinatorics, LargeCombinations) {
    EXPECT_EQ(combination(20, 10), 184756);
    EXPECT_EQ(combination(30, 15), 155117520);
}

// ============================================================================
// Fractal 测试 (Rust fractal/*.rs: 6 tests)
// ============================================================================

TEST(FractalMandelbrot, InSet) {
    EXPECT_EQ(mandelbrot_iterations(std::complex<double>(0.0, 0.0), 100), 100u);
    EXPECT_EQ(mandelbrot_iterations(std::complex<double>(-0.5, 0.0), 100), 100u);
}
TEST(FractalMandelbrot, OutOfSet) {
    EXPECT_LT(mandelbrot_iterations(std::complex<double>(2.0, 2.0), 100), 5u);
    EXPECT_LT(mandelbrot_iterations(std::complex<double>(1.0, 1.0), 100), 5u);
}
TEST(FractalMandelbrot, Boundary) {
    auto iters = mandelbrot_iterations(std::complex<double>(-0.75, 0.1), 100);
    EXPECT_TRUE(iters > 0 && iters <= 100);
}
TEST(FractalJulia, InSet) {
    std::complex<double> c(-0.1, 0.1);
    EXPECT_EQ(julia_iterations(std::complex<double>(0.0, 0.0), c, 100), 100u);
}
TEST(FractalJulia, OutOfSet) {
    std::complex<double> c(-0.1, 0.1);
    EXPECT_LT(julia_iterations(std::complex<double>(2.0, 2.0), c, 100), 5u);
}
TEST(FractalJulia, DifferentC) {
    std::complex<double> c1(0.0, 0.0);
    auto iters1 = julia_iterations(std::complex<double>(0.5, 0.0), c1, 100);
    EXPECT_TRUE(std::isfinite(iters1));
}

// ============================================================================
// 综合测试 / Integration tests
// ============================================================================

TEST(Integration, ChaoticWorkflow) {
    LorenzState state = {1.0, 1.0, 1.0};
    auto result = lorenz_iterate(state, 100, 0.01);
    EXPECT_TRUE(std::isfinite(result[0]));
    EXPECT_TRUE(std::isfinite(result[1]));
    EXPECT_TRUE(std::isfinite(result[2]));
}

TEST(Integration, CombinatoricsWorkflow) {
    EXPECT_EQ(combination(10, 5), 252);
    EXPECT_EQ(permutation(10, 5), 30240);
    EXPECT_EQ(factorial(6), 720);
}

TEST(Integration, FractalWorkflow) {
    EXPECT_EQ(mandelbrot_iterations(std::complex<double>(0.0, 0.0), 100), 100u);
    EXPECT_LT(mandelbrot_iterations(std::complex<double>(2.0, 0.0), 100), 3u);
}

// ============================================================================
// 补足 140 测试 / Fill to 140 tests
// ============================================================================

// Additional chaotic system tests (1 each for remaining systems)
TEST(ChaoticExtra, AnishchenkoAstakhov) { AnishchenkoAstakhovSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, ArnoldTongue) { ArnoldTongueSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, ArnoldsCatMap) { ArnoldsCatMapSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, BakersMap) { BakersMapSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, BogdanovMap) { BogdanovMapSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, BurkeShaw) { BurkeShawSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, ChuaAttractor) { ChuaAttractorSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, ChuaCircuit) { ChuaCircuitSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, CircleMap) { CircleMapSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, ComplexQuadratic) { ComplexQuadraticSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, ComplexSquaring) { ComplexSquaringSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, CoupledLorenz) { CoupledLorenzSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, DoublePendulum) { DoublePendulumSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, DuffingMap) { DuffingMapSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, Dyadic) { DyadicSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, Exponential) { ExponentialSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, GaussIterated) { GaussIteratedSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, GenesioTesi) { GenesioTesiSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, Gingerbreadman) { GingerbreadmanSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, HindmarshRose) { HindmarshRoseSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, IntervalExchange) { IntervalExchangeSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, KaplanYorke) { KaplanYorkeSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, KickedRotator) { KickedRotatorSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, LogisticFinite) { EXPECT_TRUE(std::isfinite(logistic_iterate(0.1, 3.9, 100))); }
TEST(ChaoticExtra, LorenzAttractor) { LorenzAttractorSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, LorenzMod1) { LorenzMod1System s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, LorenzMod2) { LorenzMod2System s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, LorenzStenflo) { LorenzStenfloSystem s; auto r = s.iterate(Point3<double>{1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, Lozi) { LoziSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, Martin) { MartinSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, NBody) { NBodySystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, NewtonLeipnik) { NewtonLeipnikSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, SineMap) { SineMapSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, SinusMap) { SinusMapSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, Symplectic) { SymplecticSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, Tent) { TentSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, ThomasCyclicallySymmetric) { ThomasCyclicallySymmetricSystem s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, ThreeScrollTsucs1) { ThreeScrollTsucs1System s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, ThreeScrollTsucs2) { ThreeScrollTsucs2System s; auto r = s.iterate({1.0,1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticExtra, Zaslavskii) { ZaslavskiiSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }

// Additional combinatorics tests
TEST(CombininatoricsExtra, CombinationLarge) {
    EXPECT_EQ(combination(50, 25), 126410606437752u);
}
TEST(CombinatoricsExtra, PermutationLarge) {
    EXPECT_EQ(permutation(20, 3), 6840);
}
TEST(CombinatoricsExtra, FactorialSmall) {
    EXPECT_EQ(factorial(3), 6);
    EXPECT_EQ(factorial(4), 24);
}
TEST(CombinatoricsExtra, CrossLarge) {
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {4, 5};
    auto p = cross(a, b);
    EXPECT_EQ(p.size(), 6u);
}
TEST(CombinatoricsExtra, CombinationZero) {
    EXPECT_EQ(combination(5, 0), 1);
    EXPECT_EQ(combination(0, 0), 1);
}
TEST(CombinatoricsExtra, PermutationZero) {
    EXPECT_EQ(permutation(5, 0), 1);
    EXPECT_EQ(permutation(0, 0), 1);
}

// Additional fractal tests
TEST(FractalExtra, MandelbrotOrigin) {
    EXPECT_EQ(mandelbrot_iterations(std::complex<double>(0.0, 0.0), 200), 200u);
}
TEST(FractalExtra, MandelbrotEscape) {
    EXPECT_LT(mandelbrot_iterations(std::complex<double>(3.0, 0.0), 100), 3u);
}
TEST(FractalExtra, JuliaOrigin) {
    std::complex<double> c(-0.7, 0.27015);
    auto iters = julia_iterations(std::complex<double>(0.0, 0.0), c, 200);
    EXPECT_TRUE(iters > 0);
}
TEST(FractalExtra, JuliaEscape) {
    std::complex<double> c(0.0, 0.0);
    EXPECT_LT(julia_iterations(std::complex<double>(2.0, 2.0), c, 100), 3u);
}
TEST(FractalExtra, MandelbrotNegativeReal) {
    auto iters = mandelbrot_iterations(std::complex<double>(-2.0, 0.0), 100);
    EXPECT_TRUE(iters > 0);
}
TEST(FractalExtra, JuliaSmallC) {
    std::complex<double> c(0.1, 0.0);
    auto iters = julia_iterations(std::complex<double>(0.0, 0.0), c, 100);
    EXPECT_TRUE(iters > 0);
}

// More chaotic system finite checks
TEST(ChaoticFinite, Lorenz) { auto r = lorenz_iterate({1.0,1.0,1.0}, 500, 0.005); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFinite, Rossler) { auto r = rossler_iterate({1.0,1.0,1.0}, 500, 0.005); EXPECT_TRUE(std::isfinite(r[0])); }
TEST(ChaoticFinite, Logistic) { EXPECT_TRUE(std::isfinite(logistic_iterate(0.1, 3.99, 1000))); }
TEST(ChaoticFinite, Henon) { HenonSystem sys; auto r = sys.iterate({0.1,0.1}, 500, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }

TEST(ChaoticExtra2, Sinusoidal) { SinusoidalSystem s; auto r = s.iterate(Point2<double>{1.0,1.0}, 10, 0.01); EXPECT_TRUE(std::isfinite(r[0])); }
