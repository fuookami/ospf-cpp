#pragma once
/// Model view /// 1:1 ∂‘”¶ Rust/core/model/basic/model_view.rs
#include <string>
#include <vector>
namespace ospf::core {
    struct ModelView {
        std::vector<std::string> variable_names;
        std::vector<std::string> constraint_names;
        std::size_t variable_count() const { return variable_names.size(); }
        std::size_t constraint_count() const { return constraint_names.size(); }
    };
}