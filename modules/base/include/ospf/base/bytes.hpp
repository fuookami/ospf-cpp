#pragma once

/// 字节工具 / Byte utilities
/// 提供位操作、编码、压缩等字节级工具。

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace ospf::base {

    // -- 位操作 / Bit operations ----------------------------------------------

    /// 检查第 n 位是否设置 / Check if bit n is set
    [[nodiscard]] constexpr bool bit_is_set(std::uint64_t value, int n) noexcept {
        return (value >> n) & 1u;
    }

    /// 设置第 n 位 / Set bit n
    [[nodiscard]] constexpr std::uint64_t bit_set(std::uint64_t value, int n) noexcept {
        return value | (1ull << n);
    }

    /// 清除第 n 位 / Clear bit n
    [[nodiscard]] constexpr std::uint64_t bit_clear(std::uint64_t value, int n) noexcept {
        return value & ~(1ull << n);
    }

    /// 翻转第 n 位 / Toggle bit n
    [[nodiscard]] constexpr std::uint64_t bit_toggle(std::uint64_t value, int n) noexcept {
        return value ^ (1ull << n);
    }

    /// popcount（置位计数） / Population count
    [[nodiscard]] constexpr int popcount(std::uint64_t value) noexcept {
        int count = 0;
        while (value) {
            count += static_cast<int>(value & 1u);
            value >>= 1;
        }
        return count;
    }

    // -- 编码 / Encoding ------------------------------------------------------

    /// 将字节转为十六进制字符串 / Convert bytes to hex string
    [[nodiscard]] inline std::string to_hex(std::span<const std::byte> bytes) {
        static constexpr char hex_chars[] = "0123456789abcdef";
        std::string result;
        result.reserve(bytes.size() * 2);
        for (auto b : bytes) {
            auto v = static_cast<std::uint8_t>(b);
            result.push_back(hex_chars[v >> 4]);
            result.push_back(hex_chars[v & 0x0f]);
        }
        return result;
    }

    /// 将十六进制字符串转为字节 / Convert hex string to bytes
    [[nodiscard]] inline std::vector<std::byte> from_hex(const std::string& hex) {
        auto hex_val = [](char c) -> std::uint8_t {
            if (c >= '0' && c <= '9') return static_cast<std::uint8_t>(c - '0');
            if (c >= 'a' && c <= 'f') return static_cast<std::uint8_t>(c - 'a' + 10);
            if (c >= 'A' && c <= 'F') return static_cast<std::uint8_t>(c - 'A' + 10);
            return 0;
        };
        std::vector<std::byte> result;
        result.reserve(hex.size() / 2);
        for (std::size_t i = 0; i + 1 < hex.size(); i += 2) {
            auto hi = hex_val(hex[i]);
            auto lo = hex_val(hex[i + 1]);
            result.push_back(static_cast<std::byte>((hi << 4) | lo));
        }
        return result;
    }

}  // namespace ospf::base
