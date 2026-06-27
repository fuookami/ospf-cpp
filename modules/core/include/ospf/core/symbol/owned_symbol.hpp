#pragma once
/// Owned symbol /// 1:1 ∂‘”¶ Rust/core/symbol/owned_symbol.rs
#include <string>
#include <memory>
namespace ospf::core {
    struct OwnedSymbol { std::string name; std::shared_ptr<void> data; };
}