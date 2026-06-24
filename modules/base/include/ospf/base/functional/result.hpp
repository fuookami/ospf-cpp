#pragma once

/// Result<T, C, E> — 三参数结果类型 / Three-parameter result type
///
/// 状态：Ok(T) | Failed(E) | Fatal(vector<E>)
/// 组合子：map / and_then / inspect_err / map_error
/// 访问：unwrap / error / errors / expect
/// 禁止抛异常；unwrap 失败调用 std::terminate。
///
/// ExResult 定义见 functional/ex_result.hpp。
/// ExResult is defined in functional/ex_result.hpp.

#include <ospf/base/error.hpp>
#include <concepts>
#include <expected>
#include <functional>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace ospf::base {

    // ---------------------------------------------------------------------------
    // Result<T, C, E>
    // ---------------------------------------------------------------------------
    template<typename T, ErrorCodeTrait C = ErrorCode, typename E = Error<C>>
    class Result {
    public:
        // -- 状态枚举（公开，供跨实例化使用） / State enum (public for cross-instantiation) --
        enum class State : std::uint8_t { Ok, Failed, Fatal };

        // -- 构造函数 / Constructors --------------------------------------------

        Result(State state, T value, E error, std::vector<E> errors)
            : state_(state), value_(std::move(value)), error_(std::move(error)),
              errors_(std::move(errors)) {}

        Result(const Result&) = default;
        Result(Result&&) noexcept = default;
        Result& operator=(const Result&) = default;
        Result& operator=(Result&&) noexcept = default;
        ~Result() = default;

        // -- 工厂 / Factories ---------------------------------------------------

        [[nodiscard]] static Result ok(T value) {
            return Result(State::Ok, std::move(value), {}, {});
        }

        [[nodiscard]] static Result failed(C code, std::string message) {
            return Result(State::Failed, {}, E(code, std::move(message)), {});
        }

        [[nodiscard]] static Result failed(E error) {
            return Result(State::Failed, {}, std::move(error), {});
        }

        [[nodiscard]] static Result fatal(C code, std::string message) {
            return Result(State::Fatal, {}, {}, { E(code, std::move(message)) });
        }

        [[nodiscard]] static Result fatal(std::vector<E> errors) {
            return Result(State::Fatal, {}, {}, std::move(errors));
        }

        // -- 状态查询 / State queries -------------------------------------------

        [[nodiscard]] bool is_ok()     const noexcept { return state_ == State::Ok; }
        [[nodiscard]] bool is_failed() const noexcept { return state_ == State::Failed || state_ == State::Fatal; }
        [[nodiscard]] bool is_fatal()  const noexcept { return state_ == State::Fatal; }

        // -- 访问 / Access ------------------------------------------------------

        /// 仅 ok，否则 terminate / Only ok, otherwise terminate
        [[nodiscard]] const T& unwrap() const {
            if (state_ != State::Ok) {
                std::terminate();
            }
            return value_;
        }

        /// 仅 failed / Only failed
        [[nodiscard]] const E& error() const {
            if (state_ != State::Failed) {
                std::terminate();
            }
            return error_;
        }

        /// 仅 fatal / Only fatal
        [[nodiscard]] const std::vector<E>& errors() const {
            if (state_ != State::Fatal) {
                std::terminate();
            }
            return errors_;
        }

        /// 期望成功，否则终止 / Expect success, otherwise terminate
        [[nodiscard]] T expect(const char* /*msg*/) const {
            if (state_ != State::Ok) {
                std::terminate();
            }
            return value_;
        }

        // -- 组合子 / Combinators -----------------------------------------------

        /// 转换成功值 / Transform success value
        template<typename F>
        [[nodiscard]] auto map(F&& f) const
            -> Result<std::invoke_result_t<F, const T&>, C, E>
        {
            using U = std::invoke_result_t<F, const T&>;
            if (state_ == State::Ok) {
                return Result<U, C, E>::ok(f(value_));
            }
            if (state_ == State::Failed) {
                return Result<U, C, E>::failed(error_);
            }
            return Result<U, C, E>::fatal(errors_);
        }

        /// 链式操作（成功时继续，失败时传递） / Chain on success
        template<typename F>
        [[nodiscard]] auto and_then(F&& f) const
            -> std::invoke_result_t<F, const T&>
        {
            using R = std::invoke_result_t<F, const T&>;
            if (state_ == State::Ok) {
                return f(value_);
            }
            if (state_ == State::Failed) {
                return R::failed(error_);
            }
            return R::fatal(errors_);
        }

        /// 失败时执行副作用 / Execute side-effect on failure
        template<typename F>
        [[nodiscard]] const Result& inspect_err(F&& f) const {
            if (state_ == State::Failed) {
                f(error_);
            } else if (state_ == State::Fatal) {
                for (const auto& e : errors_) {
                    f(e);
                }
            }
            return *this;
        }

        /// 转换错误类型 / Transform error type
        template<typename F>
        [[nodiscard]] auto map_error(F&& f) const
            -> Result<T, C, std::invoke_result_t<F, const E&>>
        {
            using E2 = std::invoke_result_t<F, const E&>;
            using R2 = Result<T, C, E2>;
            if (state_ == State::Ok) {
                return R2::ok(value_);
            }
            if (state_ == State::Failed) {
                return R2::failed(f(error_));
            }
            std::vector<E2> mapped;
            mapped.reserve(errors_.size());
            for (const auto& e : errors_) {
                mapped.push_back(f(e));
            }
            return R2::fatal(std::move(mapped));
        }

        // -- std::expected 互操作 / std::expected interop ----------------------

        /// 转为 std::expected / Convert to std::expected
        [[nodiscard]] auto to_expected() const -> std::expected<T, E> {
            if (state_ == State::Ok) {
                return std::expected<T, E>(value_);
            }
            if (state_ == State::Failed) {
                return std::unexpected<E>(error_);
            }
            return std::unexpected<E>(errors_.front());
        }

        /// 从 std::expected 构造 / Construct from std::expected
        [[nodiscard]] static Result from_expected(const std::expected<T, E>& exp) {
            if (exp.has_value()) {
                return ok(*exp);
            }
            return failed(exp.error());
        }

    private:
        State state_;
        T     value_{};
        E     error_{C::Ok, ""};
        std::vector<E> errors_;
    };

    // ---------------------------------------------------------------------------
    // 类型别名 / Type aliases (ExRet/ExTry 见 functional/ex_result.hpp)
    // ---------------------------------------------------------------------------
    using VoidResult = Result<std::monostate>;

    template<typename T>
    using Ret = Result<T>;

    using Try = VoidResult;

    // ---------------------------------------------------------------------------
    // run 辅助函数 / run helper — 顺序执行可能失败的操作
    // ---------------------------------------------------------------------------
    namespace detail {
        template<typename F>
        auto run_impl(F&& f) -> std::invoke_result_t<F> {
            return f();
        }

        template<typename F, typename... Rest>
        auto run_impl(F&& f, Rest&&... rest) -> std::invoke_result_t<F> {
            using R = std::invoke_result_t<F>;
            auto result = f();
            if constexpr (sizeof...(rest) > 0) {
                if (result.is_ok()) {
                    return result.and_then([&](const auto& val) {
                        return run_impl(std::forward<Rest>(rest)...);
                    });
                }
            }
            return result;
        }
    }  // namespace detail

    /// 顺序执行多个可能失败的操作 / Sequentially execute multiple fallible operations
    template<typename... Fs>
    auto run(Fs&&... fs) {
        return detail::run_impl(std::forward<Fs>(fs)...);
    }

}  // namespace ospf::base
