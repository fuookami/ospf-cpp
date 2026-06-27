#pragma once
/// LP export /// 1:1 ∂‘”¶ Rust core/model/intermediate/lp_export.rs
#include <string>
namespace ospf::core {
    struct LpExportConfig {
        std::string format = "lp";
        bool include_names = true;
    };
}