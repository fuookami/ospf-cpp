#pragma once
/// Elastic /// 1:1 ∂‘”¶ Rust core/model/intermediate/elastic.rs
namespace ospf::core {
    struct ElasticConfig {
        double penalty_weight = 1.0;
        bool enabled = false;
    };
}