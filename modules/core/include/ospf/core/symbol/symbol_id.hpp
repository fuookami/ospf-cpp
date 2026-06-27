#pragma once
/// Symbol ID /// 1:1 ∂‘”¶ Rust core/symbol/symbol_id.rs
#include <cstdint>
#include <string>
namespace ospf::core {
    struct SymbolId { std::uint64_t id = 0; std::string name; };
}