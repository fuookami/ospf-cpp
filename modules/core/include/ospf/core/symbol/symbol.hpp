#pragma once
/// Symbol /// 1:1 ∂‘”¶ Rust core/symbol/symbol.rs
#include <string>
#include <cstdint>
namespace ospf::core {
    enum class SymbolType : uint8_t { Variable, Constraint, Objective, Intermediate };
    struct Symbol {
        std::uint64_t id = 0;
        std::string name;
        SymbolType type = SymbolType::Variable;
        [[nodiscard]] bool operator==(const Symbol& other) const { return id == other.id; }
    };
}