#pragma once
/// Variable registry /// 1:1 ∂‘”¶ Rust/core/model/variable_registry.rs
#include <string>
#include <vector>
#include <unordered_map>
namespace ospf::core {
    struct VariableRegistry {
        std::vector<std::string> names;
        std::unordered_map<std::string, std::size_t> name_to_index;
        void add(const std::string& name) { name_to_index[name] = names.size(); names.push_back(name); }
        [[nodiscard]] std::size_t find(const std::string& name) const { auto it = name_to_index.find(name); return it != name_to_index.end() ? it->second : SIZE_MAX; }
        [[nodiscard]] std::size_t size() const { return names.size(); }
    };
}