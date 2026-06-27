#pragma once
/// Inner box /// 1:1 ∂‘”¶ Rust/core/model/flatten/inner_box.rs
#include <vector>
namespace ospf::core {
    struct InnerBox {
        std::vector<double> lower;
        std::vector<double> upper;
    };
}