/// ospf_base error/functional 测试
/// 1:1 移植 ospf-rust-base/src/error.rs 全部 #[test]，并补充 LazyError/concepts/ExResult 覆盖。

#include <gtest/gtest.h>

#include <ospf/base/error/code.hpp>
#include <ospf/base/error/error.hpp>
#include <ospf/base/error/ex_error.hpp>
#include <ospf/base/error/lazy_error.hpp>
#include <ospf/base/functional/result.hpp>
#include <ospf/base/functional/either.hpp>
#include <ospf/base/functional/range_bounds.hpp>
#include <ospf/base/functional/integer_iterator.hpp>
#include <ospf/base/functional/iterator.hpp>
#include <ospf/base/concepts/base.hpp>
#include <ospf/base/concepts/container.hpp>
#include <ospf/base/concepts/enum.hpp>
#include <ospf/base/concepts/with_default.hpp>
#include <ospf/base/concepts/with_tag.hpp>
#include <ospf/base/basic_definition.hpp>

#include <string>
#include <vector>
#include <sstream>

using namespace ospf::base;

// ============================================================================
// 辅助类型 / Helper types (ported from Rust test module)
// ============================================================================

/// 测试错误类型 / Test error type
struct TestError {
    ErrorCode code;
    std::string message;
};

/// 另一个测试错误类型 / Another test error type
struct AnotherError {
    ErrorCode code;
    int error_code;
};

// ============================================================================
// Rust #[test] 移植 / Rust test ports
// ============================================================================

// Rust: test_error_code_safe_conversion
// 验证 ErrorCode 安全转换行为
TEST(ErrorCode, SafeConversion) {
    // Rust: assert_eq!(ErrorCode::from_u8(0x2b), ErrorCode::ORModelInfeasibleOrUnbounded)
    EXPECT_EQ(error_code_from_u32(0x2b), ErrorCode::ORModelInfeasibleOrUnbounded);

    // Rust: assert_eq!(ErrorCode::from_u8(0x7f), ErrorCode::Unknown)
    EXPECT_EQ(error_code_from_u32(0x7f), ErrorCode::Unknown);

    // Rust: assert_eq!(ErrorCode::from_u64(0x1_0000), ErrorCode::Unknown)
    EXPECT_EQ(error_code_from_u32(0x10000), ErrorCode::Unknown);

    // Rust: assert_eq!(ErrorCode::IllegalArgument.to_u8(), 0x34)
    EXPECT_EQ(static_cast<std::uint32_t>(ErrorCode::IllegalArgument), 0x34u);

    // Rust: assert_eq!(ErrorCode::Other.to_u64(), 0xfe)
    EXPECT_EQ(static_cast<std::uint32_t>(ErrorCode::Other), 0xfeu);
}

// Rust: test_ex_result_warn
// 验证 ExResult 警告状态
TEST(ExResult, Warn) {
    // Rust: ExResult::warning(42u32, error!(TestError { message: "warning" }))
    auto result = ExResult<int, ErrorCode, Error<ErrorCode>>::warn(
        42, ErrorCode::Other, "warning");

    // Rust: assert!(!result.is_ok())
    EXPECT_FALSE(result.is_ok());
    // Rust: assert!(!result.is_failed())
    EXPECT_FALSE(result.is_failed());
    // Rust: assert!(result.is_warned())
    EXPECT_TRUE(result.is_warn());
    // Rust: assert_eq!(result.value(), Some(&42u32))
    EXPECT_EQ(result.unwrap(), 42);

    // Rust: let warnings = result.warnings().unwrap(); assert_eq!(warnings.len(), 1)
    const auto& warnings = result.warnings();
    ASSERT_EQ(warnings.size(), 1u);
    // Rust: assert_eq!(warnings[0].msg(), "warning".to_string())
    EXPECT_EQ(warnings[0].message, "warning");
}

// Rust: test_ex_result_fetal_alias
// 验证 ExResult 致命状态（含多个错误）
TEST(ExResult, FatalErrors) {
    // Rust: ExResult::fetal(vec![error!(...), error!(...)])
    std::vector<Error<ErrorCode>> fatal_errors;
    fatal_errors.emplace_back(ErrorCode::ApplicationFailed, "fatal-1");
    fatal_errors.emplace_back(ErrorCode::ApplicationFailed, "fatal-2");

    auto result = ExResult<int, ErrorCode, Error<ErrorCode>>::fatal(std::move(fatal_errors));

    // Rust: assert!(!result.is_ok())
    EXPECT_FALSE(result.is_ok());
    // Rust: assert!(result.is_failed())
    EXPECT_TRUE(result.is_failed());
    // Rust: assert!(result.is_fatal())
    EXPECT_TRUE(result.is_fatal());

    // Rust: let errors = result.fetal_errors().unwrap(); assert_eq!(errors.len(), 2)
    const auto& errors = result.errors();
    ASSERT_EQ(errors.size(), 2u);
    // Rust: assert_eq!(errors[0].msg(), "fatal-1")
    EXPECT_EQ(errors[0].message, "fatal-1");
    // Rust: assert_eq!(errors[1].msg(), "fatal-2")
    EXPECT_EQ(errors[1].message, "fatal-2");
}

// Rust: test_ex_result_map
// 验证 ExResult map 操作
TEST(ExResult, Map) {
    // Rust: ExResult::warning(10u32, ...).map(|value| value + 5)
    auto warned = ExResult<int, ErrorCode, Error<ErrorCode>>::warn(
        10, ErrorCode::Other, "warn");
    auto mapped = warned.map([](int value) { return value + 5; });
    EXPECT_TRUE(mapped.is_warn());
    EXPECT_EQ(mapped.unwrap(), 15);

    // Rust: ExResult::fatal_error(...).map(|value| value + 5)
    auto fatal = ExResult<int, ErrorCode, Error<ErrorCode>>::fatal(
        ErrorCode::ApplicationFailed, "fatal");
    auto mapped_fatal = fatal.map([](int value) { return value + 5; });
    EXPECT_TRUE(mapped_fatal.is_fatal());
    // Rust: assert!(mapped_fatal.value().is_none()) — fatal has no value
    EXPECT_TRUE(mapped_fatal.is_failed());
}

// ============================================================================
// 补充测试：Result<T,C,E> 三参数体系 / Additional Result tests
// ============================================================================

// Result ok/failed/fatal 状态
TEST(Result, OkFailedFatalStates) {
    auto ok = Result<int>::ok(42);
    EXPECT_TRUE(ok.is_ok());
    EXPECT_FALSE(ok.is_failed());
    EXPECT_FALSE(ok.is_fatal());
    EXPECT_EQ(ok.unwrap(), 42);

    auto failed = Result<int>::failed(ErrorCode::IllegalArgument, "bad arg");
    EXPECT_FALSE(failed.is_ok());
    EXPECT_TRUE(failed.is_failed());
    EXPECT_FALSE(failed.is_fatal());
    EXPECT_EQ(failed.error().code, ErrorCode::IllegalArgument);
    EXPECT_EQ(failed.error().message, "bad arg");

    auto fatal = Result<int>::fatal(ErrorCode::ApplicationFailed, "fatal err");
    EXPECT_FALSE(fatal.is_ok());
    EXPECT_TRUE(fatal.is_failed());
    EXPECT_TRUE(fatal.is_fatal());
    ASSERT_EQ(fatal.errors().size(), 1u);
    EXPECT_EQ(fatal.errors()[0].code, ErrorCode::ApplicationFailed);
}

// Result map
TEST(Result, Map) {
    auto ok = Result<int>::ok(10);
    auto mapped = ok.map([](int v) { return v * 2; });
    EXPECT_TRUE(mapped.is_ok());
    EXPECT_EQ(mapped.unwrap(), 20);

    auto failed = Result<int>::failed(ErrorCode::IllegalState, "bad state");
    auto mapped_fail = failed.map([](int v) { return v * 2; });
    EXPECT_TRUE(mapped_fail.is_failed());
    EXPECT_EQ(mapped_fail.error().code, ErrorCode::IllegalState);
}

// Result and_then (chaining)
TEST(Result, AndThen) {
    auto ok = Result<int>::ok(10);
    auto chained = ok.and_then([](int v) -> Result<std::string> {
        return Result<std::string>::ok(std::to_string(v));
    });
    EXPECT_TRUE(chained.is_ok());
    EXPECT_EQ(chained.unwrap(), "10");

    auto failed = Result<int>::failed(ErrorCode::DataNotFound, "missing");
    auto chained_fail = failed.and_then([](int v) -> Result<std::string> {
        return Result<std::string>::ok(std::to_string(v));
    });
    EXPECT_TRUE(chained_fail.is_failed());
    EXPECT_EQ(chained_fail.error().code, ErrorCode::DataNotFound);
}

// Result inspect_err
TEST(Result, InspectErr) {
    std::string captured_msg;
    auto failed = Result<int>::failed(ErrorCode::IllegalArgument, "bad input");
    (void)failed.inspect_err([&](const Error<ErrorCode>& e) {
        captured_msg = e.message;
    });
    EXPECT_EQ(captured_msg, "bad input");

    // ok should not trigger inspect_err
    captured_msg.clear();
    auto ok = Result<int>::ok(42);
    (void)ok.inspect_err([&](const Error<ErrorCode>& e) {
        captured_msg = e.message;
    });
    EXPECT_TRUE(captured_msg.empty());
}

// Result map_error (cross-error-code mapping)
TEST(Result, MapError) {
    auto failed = Result<int>::failed(ErrorCode::ApplicationFailed, "app failed");
    auto mapped = failed.map_error([](const Error<ErrorCode>& e) -> Error<ErrorCode> {
        return Error<ErrorCode>(ErrorCode::Unknown, "mapped: " + e.message);
    });
    EXPECT_TRUE(mapped.is_failed());
    EXPECT_EQ(mapped.error().code, ErrorCode::Unknown);
    EXPECT_EQ(mapped.error().message, "mapped: app failed");
}

// Result expect
TEST(Result, Expect) {
    auto ok = Result<int>::ok(42);
    EXPECT_EQ(ok.expect("should be ok"), 42);
}

// VoidResult (Try)
TEST(Result, VoidResult) {
    auto ok = VoidResult::ok(std::monostate{});
    EXPECT_TRUE(ok.is_ok());

    auto failed = VoidResult::failed(ErrorCode::ApplicationStopped, "stopped");
    EXPECT_TRUE(failed.is_failed());
}

// ============================================================================
// 补充测试：Error<C> 类型 / Error type tests
// ============================================================================

TEST(Error, Construction) {
    Error<ErrorCode> e(ErrorCode::IllegalArgument, "bad arg");
    EXPECT_EQ(e.code, ErrorCode::IllegalArgument);
    EXPECT_EQ(e.message, "bad arg");
}

TEST(Error, Equality) {
    Error<ErrorCode> e1(ErrorCode::IllegalArgument, "msg");
    Error<ErrorCode> e2(ErrorCode::IllegalArgument, "msg");
    Error<ErrorCode> e3(ErrorCode::Other, "msg");
    EXPECT_EQ(e1, e2);
    EXPECT_NE(e1, e3);
}

TEST(ExError, Construction) {
    ExError<ErrorCode, int> e(ErrorCode::IllegalArgument, "bad", 42);
    EXPECT_EQ(e.code, ErrorCode::IllegalArgument);
    EXPECT_EQ(e.message, "bad");
    EXPECT_EQ(e.value, 42);
}

// ============================================================================
// 补充测试：LazyError 延迟消息 / LazyError tests
// ============================================================================

TEST(LazyError, DeferredConstruction) {
    bool constructed = false;
    LazyError<ErrorCode> lazy(
        ErrorCode::ApplicationFailed,
        [&]() -> std::string {
            constructed = true;
            return "lazy message";
        });

    // 消息尚未构造 / Message not yet constructed
    EXPECT_FALSE(constructed);

    // 首次访问触发构造 / First access triggers construction
    const auto& msg = lazy.message();
    EXPECT_TRUE(constructed);
    EXPECT_EQ(msg, "lazy message");
}

TEST(LazyError, CachedMessage) {
    int call_count = 0;
    LazyError<ErrorCode> lazy(
        ErrorCode::Other,
        [&]() -> std::string {
            ++call_count;
            return "cached " + std::to_string(call_count);
        });

    // 多次访问只构造一次 / Multiple accesses only construct once
    EXPECT_EQ(lazy.message(), "cached 1");
    EXPECT_EQ(lazy.message(), "cached 1");
    EXPECT_EQ(call_count, 1);
}

// ============================================================================
// 补充测试：ErrorCodeTrait concept / Concept tests
// ============================================================================

TEST(Concepts, ErrorCodeTrait) {
    // ErrorCode 满足 ErrorCodeTrait
    static_assert(ErrorCodeTrait<ErrorCode>, "ErrorCode must satisfy ErrorCodeTrait");

    // 普通 enum class 也满足
    enum class CustomCode : std::uint32_t { Ok = 0, Fail = 1 };
    static_assert(ErrorCodeTrait<CustomCode>, "CustomCode must satisfy ErrorCodeTrait");

    // 非枚举类型不满足
    static_assert(!ErrorCodeTrait<int>, "int must not satisfy ErrorCodeTrait");
    static_assert(!ErrorCodeTrait<std::string>, "string must not satisfy ErrorCodeTrait");
}

TEST(Concepts, EnumType) {
    static_assert(EnumType<ErrorCode>, "ErrorCode must be EnumType");
    static_assert(!EnumType<int>, "int must not be EnumType");
}

TEST(Concepts, Comparable) {
    static_assert(Comparable<int>, "int must be Comparable");
    static_assert(Comparable<std::string>, "string must be Comparable");
}

TEST(Concepts, Container) {
    static_assert(Container<std::vector<int>>, "vector<int> must be Container");
}

// ============================================================================
// 补充测试：Either / RangeBounds / IntegerIterator / Iterator
// ============================================================================

TEST(Either, LeftRight) {
    auto left = Either<int, std::string>::left(42);
    EXPECT_TRUE(left.is_left());
    EXPECT_FALSE(left.is_right());
    EXPECT_EQ(left.left(), 42);

    auto right = Either<int, std::string>::right("hello");
    EXPECT_FALSE(right.is_left());
    EXPECT_TRUE(right.is_right());
    EXPECT_EQ(right.right(), "hello");
}

TEST(RangeBounds, Contains) {
    RangeBounds<int> r{0, 10};
    EXPECT_TRUE(r.contains(0));
    EXPECT_TRUE(r.contains(9));
    EXPECT_FALSE(r.contains(10));
    EXPECT_FALSE(r.contains(-1));
    EXPECT_FALSE(r.is_empty());

    RangeBounds<int> empty{5, 5};
    EXPECT_TRUE(empty.is_empty());
}

TEST(IntegerIterator, BasicRange) {
    std::vector<int> values;
    for (auto i : range(5)) {
        values.push_back(i);
    }
    ASSERT_EQ(values.size(), 5u);
    EXPECT_EQ(values[0], 0);
    EXPECT_EQ(values[4], 4);
}

TEST(IntegerIterator, StartEnd) {
    std::vector<int> values;
    for (auto i : range(3, 7)) {
        values.push_back(i);
    }
    ASSERT_EQ(values.size(), 4u);
    EXPECT_EQ(values[0], 3);
    EXPECT_EQ(values[3], 6);
}

TEST(Iterator, None) {
    std::vector<int> v{1, 3, 5, 7};
    EXPECT_TRUE(none(v, [](int x) { return x % 2 == 0; }));
    EXPECT_FALSE(none(v, [](int x) { return x > 5; }));
}

// ============================================================================
// 补充测试：run 辅助函数 / run helper tests
// ============================================================================

TEST(Run, ChainedOperations) {
    auto step1 = []() -> Result<int> { return Result<int>::ok(10); };
    auto step2 = [](int v) -> Result<int> { return Result<int>::ok(v * 2); };
    auto step3 = [](int v) -> Result<int> { return Result<int>::ok(v + 1); };

    auto result = step1().and_then(step2).and_then(step3);
    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(result.unwrap(), 21);
}

TEST(Run, ChainedWithFailure) {
    auto step1 = []() -> Result<int> { return Result<int>::ok(10); };
    auto step2 = [](int) -> Result<int> {
        return Result<int>::failed(ErrorCode::ApplicationFailed, "step2 failed");
    };
    auto step3 = [](int v) -> Result<int> { return Result<int>::ok(v + 1); };

    auto result = step1().and_then(step2).and_then(step3);
    EXPECT_TRUE(result.is_failed());
    EXPECT_EQ(result.error().code, ErrorCode::ApplicationFailed);
}

// ============================================================================
// 补充测试：std::expected 互操作 / std::expected interop
// ============================================================================

TEST(Result, ToExpected) {
    auto ok = Result<int>::ok(42);
    auto exp = ok.to_expected();
    EXPECT_TRUE(exp.has_value());
    EXPECT_EQ(*exp, 42);

    auto failed = Result<int>::failed(ErrorCode::IllegalArgument, "bad");
    auto exp_fail = failed.to_expected();
    EXPECT_FALSE(exp_fail.has_value());
    EXPECT_EQ(exp_fail.error().code, ErrorCode::IllegalArgument);
}

TEST(Result, FromExpected) {
    std::expected<int, Error<ErrorCode>> exp(42);
    auto result = Result<int>::from_expected(exp);
    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(result.unwrap(), 42);

    std::expected<int, Error<ErrorCode>> exp_err(
        std::unexpected<Error<ErrorCode>>(Error<ErrorCode>(ErrorCode::Other, "err")));
    auto result_err = Result<int>::from_expected(exp_err);
    EXPECT_TRUE(result_err.is_failed());
}

// ============================================================================
// 补充测试：ErrorCodeTrait 自定义错误码 / Custom error code with ErrorCodeTrait
// ============================================================================

namespace {
    enum class CustomErrorCode : std::uint32_t {
        Ok = 0,
        InvalidInput = 1001,
        Overflow = 1002,
    };
}

TEST(Result, CustomErrorCode) {
    static_assert(ErrorCodeTrait<CustomErrorCode>);

    using CustomResult = Result<std::string, CustomErrorCode, Error<CustomErrorCode>>;

    auto ok = CustomResult::ok("hello");
    EXPECT_TRUE(ok.is_ok());
    EXPECT_EQ(ok.unwrap(), "hello");

    auto failed = CustomResult::failed(CustomErrorCode::InvalidInput, "bad input");
    EXPECT_TRUE(failed.is_failed());
    EXPECT_EQ(failed.error().code, CustomErrorCode::InvalidInput);
}

// ============================================================================
// 补充测试：ExResult ok/fatal factories / ExResult factories
// ============================================================================

TEST(ExResult, OkFactory) {
    auto result = ExResult<int>::ok(42);
    EXPECT_TRUE(result.is_ok());
    EXPECT_FALSE(result.is_warn());
    EXPECT_EQ(result.unwrap(), 42);
    EXPECT_TRUE(result.warnings().empty());
}

TEST(ExResult, FatalFactory) {
    auto result = ExResult<int>::fatal(ErrorCode::ApplicationFailed, "fatal");
    EXPECT_TRUE(result.is_fatal());
    EXPECT_FALSE(result.is_ok());
}

TEST(ExResult, WarnMultipleWarnings) {
    std::vector<Error<ErrorCode>> warnings;
    warnings.emplace_back(ErrorCode::Other, "warn1");
    warnings.emplace_back(ErrorCode::Other, "warn2");

    auto result = ExResult<int>::warn(42, std::move(warnings));
    EXPECT_TRUE(result.is_warn());
    EXPECT_EQ(result.warnings().size(), 2u);
}
