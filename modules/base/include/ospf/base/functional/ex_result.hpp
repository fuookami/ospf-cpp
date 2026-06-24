#pragma once

/// ExResult<T, C, E> — 扩展结果类型（含 Warn 状态）/ Extended result type with Warn state
///
/// 从 result.hpp 提取，1:1 对应 Rust ExResult 枚举。
/// Extracted from result.hpp, 1:1 mapping to Rust ExResult enum.
///
/// 状态：Ok | Failed | Fatal | Warn
/// Warn 状态持有成功值 + 警告列表。

#include <ospf/base/error.hpp>
#include <ospf/base/functional/result.hpp>
#include <string>
#include <vector>

namespace ospf::base {

    // ============================================================================
    // ExResult<T, C, E> — 扩展结果（含 Warn 状态）/ Extended result with Warn state
    // ============================================================================
    template<typename T, ErrorCodeTrait C = ErrorCode, typename E = Error<C>>
    class ExResult : public Result<T, C, E> {
    public:
        using Base = Result<T, C, E>;

        // -- 工厂 / Factories ---------------------------------------------------

        [[nodiscard]] static ExResult ok(T value) {
            return ExResult(Base::ok(std::move(value)), {});
        }

        [[nodiscard]] static ExResult failed(C code, std::string message) {
            return ExResult(Base::failed(code, std::move(message)), {});
        }

        [[nodiscard]] static ExResult failed(E error) {
            return ExResult(Base::failed(std::move(error)), {});
        }

        [[nodiscard]] static ExResult fatal(C code, std::string message) {
            return ExResult(Base::fatal(code, std::move(message)), {});
        }

        [[nodiscard]] static ExResult fatal(std::vector<E> errors) {
            return ExResult(Base::fatal(std::move(errors)), {});
        }

        /// 警告状态（成功值 + 警告列表）/ Warn state (success value + warning list)
        [[nodiscard]] static ExResult warn(T value, C code, std::string message) {
            return ExResult(Base::ok(value), { E(code, std::move(message)) });
        }

        [[nodiscard]] static ExResult warn(T value, E warning) {
            return ExResult(Base::ok(value), { std::move(warning) });
        }

        [[nodiscard]] static ExResult warn(T value, std::vector<E> warnings) {
            return ExResult(Base::ok(value), std::move(warnings));
        }

        // -- 状态查询 / State queries -------------------------------------------
        // Rust ExResult: is_ok 仅 Ok（非 Warn），is_failed 仅 Failed+Fatal（非 Warn）

        [[nodiscard]] bool is_ok()     const noexcept { return !is_warn() && !Base::is_failed() && !Base::is_fatal(); }
        [[nodiscard]] bool is_failed() const noexcept { return !is_warn() && Base::is_failed(); }
        [[nodiscard]] bool is_fatal()  const noexcept { return !is_warn() && Base::is_fatal(); }
        [[nodiscard]] bool is_warn()   const noexcept { return !warnings_.empty() && Base::is_ok(); }

        // -- 访问 / Access ------------------------------------------------------

        /// 获取成功值（Ok 或 Warn 状态均可）/ Get success value (Ok or Warn)
        [[nodiscard]] const T* value() const noexcept {
            if (Base::is_ok()) {
                return &Base::unwrap();
            }
            return nullptr;
        }

        [[nodiscard]] const std::vector<E>& warnings() const {
            return warnings_;
        }

        // -- 组合子 / Combinators -----------------------------------------------

        template<typename F>
        [[nodiscard]] auto map(F&& f) const -> ExResult<std::invoke_result_t<F, const T&>, C, E> {
            using U = std::invoke_result_t<F, const T&>;
            if (is_warn()) {
                return ExResult<U, C, E>::warn(f(Base::unwrap()), std::vector<E>(warnings_));
            }
            if (Base::is_ok()) {
                return ExResult<U, C, E>::ok(f(Base::unwrap()));
            }
            if (Base::is_fatal()) {
                return ExResult<U, C, E>::fatal(Base::errors());
            }
            return ExResult<U, C, E>::failed(Base::error());
        }

    private:
        ExResult(Base base, std::vector<E> warnings)
            : Base(std::move(base)), warnings_(std::move(warnings)) {}

        std::vector<E> warnings_;
    };

    // ---------------------------------------------------------------------------
    // 扩展类型别名 / Extended type aliases
    // ---------------------------------------------------------------------------
    template<typename T>
    using ExRet = ExResult<T>;

    using ExTry = ExResult<std::monostate>;

}  // namespace ospf::base
