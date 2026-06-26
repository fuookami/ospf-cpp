#pragma once
/// Common types /// 1:1 对应 Rust example/core/common.rs
#include <string>
#include <vector>
namespace ospf::example {
    struct ExampleResult {
        std::string name;
        double objective = 0.0;
        bool feasible = false;
    };
}
