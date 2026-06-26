#pragma once
/// ²ã¸³ÖµÈÝÁ¿ / Layer assignment capacity
namespace ospf::framework::bpp3d { struct LayerCapacity { double max_depth = 0.0; double used_depth = 0.0; [[nodiscard]] double remaining() const { return max_depth - used_depth; } }; }
