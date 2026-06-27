#pragma once
/// Unit conversion value /// 1:1 ∂‘”¶ Rust quantities/unit/conversion_value.rs
#include <optional>
namespace ospf::quantities {
    struct ConversionValue {
        double factor = 1.0;
        double offset = 0.0;
        [[nodiscard]] double convert(double v) const { return v * factor + offset; }
    };
}