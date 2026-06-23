#pragma once

/// 符号变量 / Symbolic variable

#include <cstdint>
#include <string>
#include <ostream>

namespace ospf::math::symbol {

    /// 符号变量 / Symbolic variable
    class Variable {
    public:
        explicit Variable(std::string name, std::uint64_t id = 0)
            : name_(std::move(name)), id_(id) {}

        [[nodiscard]] const std::string& name() const noexcept { return name_; }
        [[nodiscard]] std::uint64_t id() const noexcept { return id_; }

        [[nodiscard]] bool operator==(const Variable& other) const noexcept {
            return id_ == other.id_ && name_ == other.name_;
        }

        [[nodiscard]] bool operator<(const Variable& other) const noexcept {
            if (id_ != other.id_) return id_ < other.id_;
            return name_ < other.name_;
        }

        friend std::ostream& operator<<(std::ostream& os, const Variable& v) {
            return os << v.name_;
        }

    private:
        std::string name_;
        std::uint64_t id_;
    };

}  // namespace ospf::math::symbol
