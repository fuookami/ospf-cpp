# ospf-cpp

[English](README.md) | [中文](README_ch.md)

OSPF（优化求解编程框架）的 C++23 实现。

## 概述

ospf-cpp 是一个全面的优化框架，提供：

- **核心建模** — 变量、约束、目标、MetaModel
- **数学库** — 代数、几何、符号数学、混沌系统、组合数学、分形
- **物理量** — 带维度检查的物理量
- **多维数组** — N 维数组容器
- **领域框架** — BPP3D（3D 装箱）、CSP1D（1D 下料）、Gantt 排程

## 构建

### 前置条件

- CMake 3.21+
- Visual Studio 2022 (MSVC 19.50+)
- vcpkg（清单模式）

### 命令

```bash
cmake --preset default
cmake --build --preset default
ctest --preset default --output-on-failure -C Release
```

## 模块结构

```
modules/
├── base/          — 错误处理、函数式、概念、工具
├── multiarray/    — N 维数组容器
├── math/          — 代数、几何、符号、混沌、组合、分形
├── quantities/    — 带维度检查的物理量
├── core/          — 符号、变量、模型、求解器
└── framework/     — 上下文、管线、列生成、Benders
    ├── bpp3d/     — 3D 装箱问题
    ├── csp1d/     — 1D 下料问题
    └── gantt/     — Gantt 排程
```

## 关键类型

### 错误处理

- `Result<T, C, E>` — 三参数结果类型（Ok/Failed/Fatal）
- `ExResult<T, C, E>` — 扩展结果，含 Warn 状态
- `ErrorCodeTrait` — 错误码类型概念

### 数学

- `Vector<T, N>` — 固定大小数学向量
- `Matrix<T, Rows, Cols>` — 固定大小数学矩阵
- `Shape<N>` / `DynShape` — 多维数组形状
- `Expression` / `Monomial` / `Polynomial` — 符号表达式

### 物理量

- `Quantity<V, Dim>` — 带维度检查的物理量
- `Dimension` — 物理维度（M, L, T, I, Θ, N, J）

### 核心

- `MetaModel` — 管理变量、约束、目标的核心模型
- `SolverTrait` — 抽象求解器接口
- `StubSolver` — 返回可预测解的测试求解器

## 示例

参见 `examples/` 目录中的可运行演示：

- `core_modeling` — 变量/约束/目标建模
- `bpp3d_demo` — 3D 装箱
- `csp1d_demo` — 1D 下料
- `gantt_demo` — Gantt 排程

## 错误码

| 范围 | 模块 |
|------|------|
| 0x00-0xFF | 基础（ErrorCode） |
| 0x1000+ | 领域模块（BPP3D、CSP1D、Gantt） |

## 许可证

参见 LICENSE 文件。
