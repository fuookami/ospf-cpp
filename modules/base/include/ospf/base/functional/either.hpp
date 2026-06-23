#pragma once

/// Either<L, R> — 二选一类型 / Either-or type

#include <variant>

namespace ospf::base {

    /// Either 类型：左值或右值 / Either type: left or right value
    template<typename L, typename R>
    class Either {
    public:
        [[nodiscard]] static Either left(L value) {
            return Either(std::variant<L, R>(std::in_place_index<0>, std::move(value)));
        }

        [[nodiscard]] static Either right(R value) {
            return Either(std::variant<L, R>(std::in_place_index<1>, std::move(value)));
        }

        [[nodiscard]] bool is_left()  const noexcept { return data_.index() == 0; }
        [[nodiscard]] bool is_right() const noexcept { return data_.index() == 1; }

        [[nodiscard]] const L& left()  const { return std::get<0>(data_); }
        [[nodiscard]] const R& right() const { return std::get<1>(data_); }

    private:
        explicit Either(std::variant<L, R> data) : data_(std::move(data)) {}
        std::variant<L, R> data_;
    };

}  // namespace ospf::base
