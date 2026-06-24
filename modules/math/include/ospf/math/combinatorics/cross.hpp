#pragma once
/// 笛卡尔积 / Cartesian product /// 1:1 对应 Rust combinatorics/cross.rs
#include <vector>
namespace ospf::math::combinatorics {
    template<typename T>
    [[nodiscard]] std::vector<std::pair<T, T>> cross(
        const std::vector<T>& a, const std::vector<T>& b)
    {
        std::vector<std::pair<T, T>> result;
        for (const auto& x : a)
            for (const auto& y : b)
                result.emplace_back(x, y);
        return result;
    }
}
