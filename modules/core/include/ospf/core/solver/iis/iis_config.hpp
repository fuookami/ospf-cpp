#pragma once
/// IIS config /// 1:1 ∂‘”¶ Rust/core/solver/iis/iis_config.rs
namespace ospf::core {
    struct IisConfig {
        bool verbose = false;
        int max_iterations = 100;
    };
}