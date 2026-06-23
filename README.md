# ospf-cpp

[English](README.md) | [中文](README_ch.md)

C++23 implementation of the OSPF (Optimization Solver Programming Framework).

## Overview

ospf-cpp is a comprehensive optimization framework providing:

- **Core modeling** — Variables, constraints, objectives, MetaModel
- **Math library** — Algebra, geometry, symbolic math, chaotic systems, combinatorics, fractals
- **Quantities** — Physical quantities with dimension checking
- **Multi-array** — N-dimensional array container
- **Domain frameworks** — BPP3D (3D bin packing), CSP1D (1D cutting stock), Gantt scheduling

## Build

### Prerequisites

- CMake 3.21+
- Visual Studio 2022 (MSVC 19.50+)
- vcpkg (manifest mode)

### Commands

```bash
cmake --preset default
cmake --build --preset default
ctest --preset default --output-on-failure -C Release
```

## Module Structure

```
modules/
├── base/          — Error handling, functional, concepts, utilities
├── multiarray/    — N-dimensional array container
├── math/          — Algebra, geometry, symbolic, chaotic, combinatorics, fractal
├── quantities/    — Physical quantities with dimension checking
├── core/          — Symbol, variable, model, solver
└── framework/     — Context, pipeline, column generation, Benders
    ├── bpp3d/     — 3D Bin Packing Problem
    ├── csp1d/     — 1D Cutting Stock Problem
    └── gantt/     — Gantt scheduling
```

## Key Types

### Error Handling

- `Result<T, C, E>` — Three-parameter result type (Ok/Failed/Fatal)
- `ExResult<T, C, E>` — Extended result with Warn state
- `ErrorCodeTrait` — Concept for error code types

### Math

- `Vector<T, N>` — Fixed-size mathematical vector
- `Matrix<T, Rows, Cols>` — Fixed-size mathematical matrix
- `Shape<N>` / `DynShape` — Multi-dimensional array shapes
- `Expression` / `Monomial` / `Polynomial` — Symbolic expressions

### Quantities

- `Quantity<V, Dim>` — Physical quantity with dimension checking
- `Dimension` — Physical dimension (M, L, T, I, Θ, N, J)

### Core

- `MetaModel` — Core model managing variables, constraints, objectives
- `SolverTrait` — Abstract solver interface
- `StubSolver` — Test solver returning predictable solutions

## Examples

See `examples/` for runnable demos:

- `core_modeling` — Variable/constraint/objective modeling
- `bpp3d_demo` — 3D bin packing
- `csp1d_demo` — 1D cutting stock
- `gantt_demo` — Gantt scheduling

## Error Codes

| Range | Module |
|-------|--------|
| 0x00-0xFF | Base (ErrorCode) |
| 0x1000+ | Domain modules (BPP3D, CSP1D, Gantt) |

## License

See LICENSE file.
