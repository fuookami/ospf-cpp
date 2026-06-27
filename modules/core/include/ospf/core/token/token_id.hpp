#pragma once
/// Token ID /// 1:1 对应 Rust core/token/token_id.rs
/// Token 唯一标识符

#include <cstdint>
#include <string>
#include <functional>

namespace ospf::core {

    /// Token ID / Token identifier
    struct TokenId {
        std::uint64_t id = 0;
        std::string name;

        [[nodiscard]] bool operator==(const TokenId& other) const noexcept {
            return id == other.id;
        }

        [[nodiscard]] bool operator!=(const TokenId& other) const noexcept {
            return id != other.id;
        }

        [[nodiscard]] bool operator<(const TokenId& other) const noexcept {
            return id < other.id;
        }
    };

    /// Token ID 生成器 / Token ID generator
    class TokenIdGenerator {
    public:
        [[nodiscard]] static TokenIdGenerator& instance() noexcept {
            static TokenIdGenerator gen;
            return gen;
        }

        [[nodiscard]] TokenId next(const std::string& name = "") {
            return TokenId{next_id_++, name};
        }

        void reset() noexcept { next_id_ = 0; }

    private:
        std::uint64_t next_id_ = 0;
    };

}  // namespace ospf::core
