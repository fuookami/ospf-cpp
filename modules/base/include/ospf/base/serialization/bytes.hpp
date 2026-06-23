#pragma once

/// 字节序列化 / Byte serialization

#include <cstddef>
#include <cstring>
#include <span>
#include <vector>

namespace ospf::base {

    /// 将 POD 类型序列化为字节 / Serialize POD type to bytes
    template<typename T>
    [[nodiscard]] std::vector<std::byte> to_bytes(const T& value) {
        static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        std::vector<std::byte> result(sizeof(T));
        std::memcpy(result.data(), &value, sizeof(T));
        return result;
    }

    /// 从字节反序列化 POD 类型 / Deserialize POD type from bytes
    template<typename T>
    [[nodiscard]] T from_bytes(std::span<const std::byte> bytes) {
        static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        T value;
        std::memcpy(&value, bytes.data(), sizeof(T));
        return value;
    }

    /// 序列化 vector 为字节 / Serialize vector to bytes
    template<typename T>
    [[nodiscard]] std::vector<std::byte> to_bytes(const std::vector<T>& vec) {
        static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        std::size_t size = vec.size();
        std::vector<std::byte> result(sizeof(std::size_t) + vec.size() * sizeof(T));
        std::memcpy(result.data(), &size, sizeof(std::size_t));
        std::memcpy(result.data() + sizeof(std::size_t), vec.data(), vec.size() * sizeof(T));
        return result;
    }

    /// 从字节反序列化 vector / Deserialize vector from bytes
    template<typename T>
    [[nodiscard]] std::vector<T> vector_from_bytes(std::span<const std::byte> bytes) {
        static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        std::size_t size;
        std::memcpy(&size, bytes.data(), sizeof(std::size_t));
        std::vector<T> result(size);
        std::memcpy(result.data(), bytes.data() + sizeof(std::size_t), size * sizeof(T));
        return result;
    }

}  // namespace ospf::base
