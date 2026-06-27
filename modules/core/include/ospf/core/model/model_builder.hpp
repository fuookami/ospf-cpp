#pragma once
/// Model builder /// 1:1 ∂‘”¶ Rust core/model/model_builder.rs
#include <ospf/core/model.hpp>
#include <string>
namespace ospf::core {
    struct ModelBuilder {
        std::string name = "unnamed";
        ModelBuilder& set_name(const std::string& n) { name = n; return *this; }
        [[nodiscard]] MetaModel build() { return MetaModel(name); }
    };
}