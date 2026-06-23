#pragma once

/// 符号系统 / Symbol system
/// 符号是模型中变量、约束、目标的标识符。

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>

namespace ospf::core {

    /// 符号类型 / Symbol type
    enum class SymbolType : std::uint8_t {
        Variable,    ///< 变量 / Variable
        Constraint,  ///< 约束 / Constraint
        Objective,   ///< 目标 / Objective
    };

    /// 符号 / Symbol: identifier in the model
    class Symbol {
    public:
        Symbol(std::string name, SymbolType type, std::uint64_t id)
            : name_(std::move(name)), type_(type), id_(id) {}

        [[nodiscard]] const std::string& name() const noexcept { return name_; }
        [[nodiscard]] SymbolType type() const noexcept { return type_; }
        [[nodiscard]] std::uint64_t id() const noexcept { return id_; }

        [[nodiscard]] bool operator==(const Symbol& other) const noexcept {
            return id_ == other.id_;
        }

        [[nodiscard]] bool operator<(const Symbol& other) const noexcept {
            return id_ < other.id_;
        }

    private:
        std::string name_;
        SymbolType type_;
        std::uint64_t id_;
    };

    /// 符号注册表 / Symbol registry
    class SymbolRegistry {
    public:
        [[nodiscard]] static SymbolRegistry& instance() {
            static SymbolRegistry reg;
            return reg;
        }

        Symbol create_symbol(const std::string& name, SymbolType type) {
            std::lock_guard lock(mutex_);
            auto id = next_id_++;
            return Symbol(name, type, id);
        }

        [[nodiscard]] std::uint64_t count() const {
            std::lock_guard lock(mutex_);
            return next_id_;
        }

        void reset() {
            std::lock_guard lock(mutex_);
            next_id_ = 0;
        }

    private:
        SymbolRegistry() = default;
        mutable std::mutex mutex_;
        std::uint64_t next_id_ = 0;
    };

}  // namespace ospf::core
