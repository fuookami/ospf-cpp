#pragma once
/// Expression registry /// 1:1 ∂‘”¶ Rust/core/model/expression_registry.rs
#include <string>
#include <vector>
namespace ospf::core {
    struct ExpressionRegistry {
        std::vector<std::string> names;
        void add(const std::string& name) { names.push_back(name); }
        [[nodiscard]] std::size_t size() const { return names.size(); }
    };
}