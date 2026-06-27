#pragma once
/// Model status
#include <cstdint>
namespace ospf::core {
    enum class ModelStatus : uint8_t { Created, Building, Solving, Solved, Error };
}