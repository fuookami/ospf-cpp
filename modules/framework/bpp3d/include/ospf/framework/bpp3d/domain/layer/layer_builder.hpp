#pragma once
#include <ospf/framework/bpp3d/domain/layer/layer.hpp>
namespace ospf::framework::bpp3d {
    struct LayerBuilder {
        int index = 0;
        double max_height = 0.0;
        LayerBuilder& set_index(int i) { index = i; return *this; }
        LayerBuilder& set_max_height(double h) { max_height = h; return *this; }
        [[nodiscard]] Layer build() const { Layer l; l.index = index; l.max_height = max_height; return l; }
    };
}