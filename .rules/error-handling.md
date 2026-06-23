# 错误处理规范

## 1. 核心原则

**整个项目统一使用返回错误（Result 模式），不抛出异常。**

所有可能失败的操作都应返回 `Result<T>` 或 `VoidResult`，而不是抛出异常。

## 2. 错误类型体系

### 2.1 基础类型（ospf_base）

- `ErrorCode` - 错误码枚举，定义所有标准错误码
- `Error` - 错误类，包含 `code` 和 `message`
- `ErrorList` - 多个错误的聚合，用于 `Fatal` 场景

### 2.2 结果类型（ospf_base）

- `Result<T>` - 核心结果类型，内部持有 `std::variant<OkTag<T>, FailedTag, FatalTag>`
  - `Result<T>::Ok(T value)` - 成功结果，包含值
  - `Result<T>::Failed(Error error)` - 失败结果，包含单个错误
  - `Result<T>::Fatal(ErrorList errors)` - 致命结果，包含多个错误
- `VoidResult` - 无返回值的结果：`Result<std::monostate>`
- `ExResult<T>` - 扩展结果类型，在 `Result<T>` 基础上增加：
  - `Result<T>::Warn(T value, Error warning)` - 警告结果，同时包含值和警告

### 2.3 类型别名

```cpp
// 无返回值
using VoidResult = Result<std::monostate>;

// 带返回值
template<typename T>
using Ret = Result<T>;
```

## 3. 使用规范

### 3.1 函数签名

```cpp
// 正确：返回 Result
auto parse(const std::string &input) -> Result<ParsedData> {
    if (!is_valid(input)) {
        return Result<ParsedData>::failed(
            ErrorCode::IllegalArgument,
            "Invalid input format / 输入格式无效"
        );
    }
    return Result<ParsedData>::ok(parse_data(input));
}

// 正确：返回 VoidResult（无有意义返回值）
auto save(const Data &data) -> VoidResult {
    if (!repository.save(data)) {
        return VoidResult::failed(
            ErrorCode::ApplicationFailed,
            "Save failed / 保存失败"
        );
    }
    return VoidResult::ok();
}

// 错误：抛出异常
auto parse(const std::string &input) -> ParsedData {
    if (!is_valid(input)) {
        throw std::invalid_argument("Invalid input format");
    }
    return parse_data(input);
}
```

### 3.2 错误传播

使用 `and_then` 串联多个可能失败的操作，使用 `map` 转换成功值：

```cpp
auto process(const std::string &input) -> Result<Output> {
    return validate(input)
        .and_then([](auto &&validated) { return transform(validated); })
        .and_then([](auto &&transformed) { return save(transformed); });
}
```

或使用辅助函数 `run` 顺序执行：

```cpp
auto process(const std::string &input) -> Result<Output> {
    return run(
        [&] { return validate(input); },
        [&](auto &&validated) { return transform(validated); },
        [&](auto &&transformed) { return save(transformed); }
    );
}
```

### 3.3 错误映射

使用 `map` 转换成功值，使用 `map_error` 转换错误，使用 `inspect_err` 在失败时执行副作用：

```cpp
auto result = parse(input)
    .map([](auto &&data) { return to_string(data); })
    .inspect_err([](const auto &error) { spdlog::error("Parse failed: {}", error.message); });
```

### 3.4 工厂函数

使用提供的工厂函数创建结果：

```cpp
// 成功
Result<T>::ok(value)
VoidResult::ok()

// 失败
Result<T>::failed(ErrorCode::IllegalArgument, "message")
VoidResult::failed(ErrorCode::IllegalArgument, "message")

// 致命
Result<T>::fatal(ErrorCode::ApplicationError, "fatal message")
Result<T>::fatal(ErrorList{error1, error2})

// 警告
ExResult<T>::warn(value, ErrorCode::Other, "warning message")
```

### 3.5 与 std::expected 的互操作

如果编译器支持 C++23 `std::expected`，可提供互操作转换：

```cpp
// Result<T> -> std::expected<T, Error>
auto to_expected(const Result<T> &result) -> std::expected<T, Error>;

// std::expected<T, E> -> Result<T>
auto from_expected(const std::expected<T, E> &exp) -> Result<T>;
```

## 4. 禁止的模式

### 4.1 禁止抛出异常

```cpp
// 禁止
throw std::invalid_argument("...");
throw std::runtime_error("...");
throw std::logic_error("...");
throw std::out_of_range("...");
```

### 4.2 禁止忽略错误

```cpp
// 禁止：Result 未检查即丢弃
save(data);  // 警告：返回的 VoidResult 被忽略

// 正确：显式处理或向上传播
co_await save(data);  // 协程场景
auto _ = save(data);  // 显式忽略（使用下划线或 [[maybe_unused]]）
save(data).expect("save must succeed");  // 测试中可接受
```

### 4.3 禁止在 Result 处理中抛异常

```cpp
// 禁止
auto result = some_operation();
if (result.is_failed()) {
    throw std::runtime_error(result.error().message);
}
```

## 5. 允许的例外情况

### 5.1 测试代码

测试代码中可以使用异常来：
- 模拟失败场景
- 断言预期行为
- 测试 stub 实现

```cpp
// 测试中允许
auto method() -> Type override {
    throw std::runtime_error("stub");
}
```

### 5.2 外部库交互

与不支持 Result 模式的外部库交互时，可以在边界处捕获异常并转换为 Result：

```cpp
auto external_call() -> Result<Response> {
    try {
        return Result<Response>::ok(external_library.do_something());
    } catch (const ExternalException &e) {
        return Result<Response>::failed(
            ErrorCode::Other,
            fmt::format("External call failed / 外部调用失败: {}", e.what())
        );
    }
}
```

### 5.3 不可恢复的系统级错误

以下场景因系统约束保留 `throw` / `abort`，不属于迁移范围：

- **内存分配失败**：`std::bad_alloc` 在 OOM 时无法通过返回值恢复。
- **构造函数中的不变量违反**：当对象根本无法构造时（如参数校验失败导致对象处于无效状态），允许在构造函数中抛异常。
- **STL / 第三方库契约违反**：如 `std::vector::at()` 越界，属于编程错误而非业务错误。

## 6. ErrorCode 扩展

当现有 ErrorCode 不足以表达错误类型时：

1. 首先检查是否可以复用现有 ErrorCode
2. 如果需要新的领域特定错误码，在相应模块定义扩展枚举
3. 确保错误码值不与现有 ErrorCode 冲突

## 7. 错误消息规范

- 使用简洁明了的中英双语消息
- 中文在前，英文在后，用 ` / ` 分隔
- 包含足够的上下文信息（如参数值、状态）
- 避免暴露内部实现细节
- 格式：`"操作失败：原因 / Operation failed: reason"`
- 使用 `fmt::format` 构建动态消息，避免字符串拼接
