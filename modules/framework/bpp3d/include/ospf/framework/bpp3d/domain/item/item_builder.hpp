#pragma once
/// BPP3D domain/item/item_builder
#include <ospf/framework/bpp3d/domain/item/item.hpp>
namespace ospf::framework::bpp3d {
    struct ItemBuilder {
        std::string id;
        double w = 0, h = 0, d = 0, weight = 0;
        int qty = 1;
        ItemBuilder& set_id(const std::string& i) { id = i; return *this; }
        ItemBuilder& set_size(double w_, double h_, double d_) { w = w_; h = h_; d = d_; return *this; }
        ItemBuilder& set_weight(double wt) { weight = wt; return *this; }
        ItemBuilder& set_quantity(int q) { qty = q; return *this; }
        [[nodiscard]] Item build() const { return {id, w, h, d, weight, qty}; }
    };
}