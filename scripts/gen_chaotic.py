#!/usr/bin/env python3
import os

base = "E:/workspace/ospf/ospf-cpp/modules/math/include/ospf/math/chaotic"

systems = {
    "arneodo": ("Arneodo", {"a": -5.5, "b": 3.5, "c": 1.0}),
    "bouali": ("Bouali", {"a": 0.1, "b": 1.0, "c": 1.0}),
    "burke_shaw": ("BurkeShaw", {"s": 10.0, "v": 4.267}),
    "chua_attractor": ("ChuaAttractor", {"a": 15.6, "b": 28.0, "c": -1.143, "d": -0.714}),
    "chua_circuit": ("ChuaCircuit", {"a": 15.6, "b": 28.0, "c": -1.143, "d": -0.714}),
    "circuit_chaotic": ("CircuitChaotic", {"a": 1.0, "b": 1.0, "c": 1.0}),
    "coullet": ("Coullet", {"a": 0.8, "b": -1.1, "c": -1.0}),
    "coupled_lorenz": ("CoupledLorenz", {"sigma": 10.0, "rho": 28.0, "beta": 8.0/3.0, "k": 0.1}),
    "dadras": ("Dadras", {"a": 3.0, "b": 2.7, "c": 1.7, "d": 2.0, "e": 9.0}),
    "dequan_li": ("DequanLi", {"a": 40.0, "b": 1.833, "c": 0.16, "d": 0.65, "e": 55.0}),
    "four_scroll_hyper_chaotic": ("FourScrollHyperChaotic", {"a": 10.0, "b": 40.0, "c": 2.0, "d": 2.5}),
    "four_wing": ("FourWing", {"a": 4.0, "b": 6.0, "c": 1.0}),
    "hadley": ("Hadley", {"a": 0.2, "b": 4.0, "c": 8.0, "d": 1.0}),
    "halvorsen": ("Halvorsen", {"a": 1.89}),
    "hindmarsh_rose": ("HindmarshRose", {"a": 1.0, "b": 3.0, "c": 1.0, "d": 5.0}),
    "ikeda": ("Ikeda", {"u": 0.9, "k": 1.0}),
    "interval_exchange": ("IntervalExchange", {"a": 1.0, "b": 1.0}),
    "kaplan_yorke": ("KaplanYorke", {"a": 0.1, "b": 0.1}),
    "kicked_rotator": ("KickedRotator", {"k": 1.0}),
    "liu_chen": ("LiuChen", {"a": 2.0, "b": 1.0, "c": 4.0}),
    "lorenz84": ("Lorenz84", {"a": 0.95, "b": 7.91, "c": 4.83, "d": 4.0}),
    "lorenz96": ("Lorenz96", {"F": 8.0, "n": 8.0}),
    "lorenz_attractor": ("LorenzAttractor", {"sigma": 10.0, "rho": 28.0, "beta": 8.0/3.0}),
    "lorenz_mod1": ("LorenzMod1", {"a": 10.0, "b": 28.0, "c": 8.0/3.0, "d": 1.0}),
    "lorenz_mod2": ("LorenzMod2", {"a": 10.0, "b": 28.0, "c": 8.0/3.0, "d": 0.5}),
    "lorenz_stenflo": ("LorenzStenflo", {"a": 10.0, "b": 28.0, "c": 8.0/3.0, "d": 1.0}),
    "lotka_volterra": ("LotkaVolterra", {"a": 1.0, "b": 0.1, "c": 1.5, "d": 0.75}),
    "lozi": ("Lozi", {"a": 1.7, "b": 0.5}),
    "lu_chen_attractor": ("LuChenAttractor", {"a": 36.0, "b": 3.0, "c": 20.0}),
    "lu_chen_system": ("LuChenSystem", {"a": 40.0, "b": 2.0, "c": 55.0}),
    "martin": ("Martin", {"a": 1.0, "b": 1.0}),
    "n_body": ("NBody", {"G": 1.0, "n": 3.0}),
    "newton_leipnik": ("NewtonLeipnik", {"a": 0.4, "b": 0.175}),
    "nose_hoover": ("NoseHoover", {"a": 1.0}),
    "qi_attractor": ("QiAttractor", {"a": 38.0, "b": 8.0, "c": 80.0/3.0}),
    "qi_chen": ("QiChen", {"a": 38.0, "b": 2.0, "c": 80.0/3.0}),
    "rucklidge": ("Rucklidge", {"k": 2.0, "a": 6.7}),
    "sakarya": ("Sakarya", {"a": 0.4, "b": 0.3}),
    "shimizu_morioka": ("ShimizuMorioka", {"a": 0.75, "b": 0.45}),
    "singer": ("Singer", {"a": 1.0}),
    "symplectic": ("Symplectic", {"a": 1.0, "b": 1.0}),
    "thomas": ("Thomas", {"b": 0.21}),
    "thomas_cyclically_symmetric": ("ThomasCyclicallySymmetric", {"b": 0.21}),
    "three_scroll_tsucs1": ("ThreeScrollTsucs1", {"a": 40.0, "b": 0.833, "c": 0.5, "d": 0.65, "e": 20.0}),
    "three_scroll_tsucs2": ("ThreeScrollTsucs2", {"a": 40.0, "b": 0.833, "c": 0.5, "d": 0.65, "e": 20.0}),
    "tinkerbell": ("Tinkerbell", {"a": 0.9, "b": -0.6013, "c": 2.0, "d": 0.5}),
    "van_der_pol": ("VanDerPol", {"mu": 1.0}),
    "wang_sun": ("WangSun", {"a": 10.0, "b": 40.0, "c": 2.0, "d": 2.5}),
    "wimol_banlue": ("WimolBanlue", {"a": 1.0, "b": 1.0}),
    "yu_wang": ("YuWang", {"a": 10.0, "b": 40.0, "c": 2.0}),
    "zaslavskii": ("Zaslavskii", {"epsilon": 0.5, "r": 1.0, "d": 0.7}),
}

count = 0
for fname, (classname, params) in systems.items():
    path = os.path.join(base, f"{fname}.hpp")
    param_fields = "\n        ".join([f"double {k} = {v};" for k, v in params.items()])

    content = f"""#pragma once
/// {classname} 混沌系统 / {classname} chaotic system
/// 1:1 对应 Rust chaotic/{fname}.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>

namespace ospf::math::chaotic {{

    struct {classname}Params {{
        {param_fields}

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {{
            // TODO: 实现 {classname} 方程 / Implement {classname} equations
            return {{S{{0}}, S{{0}}, S{{0}}}};
        }}
    }};

    static constexpr char k{classname}Name[] = "{fname}";
    using {classname}System = ChaoticSystem3D<{classname}Params, k{classname}Name>;

}}  // namespace ospf::math::chaotic
"""
    with open(path, 'w') as f:
        f.write(content)
    count += 1

print(f"Generated {count} chaotic system headers")
