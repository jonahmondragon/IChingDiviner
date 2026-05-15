---
description: Agent guidelines for building, linting, testing, and code style
mode: primary
agent: build
---

# Build, Lint, and Test Commands

- Build project:
  ```bash
  mkdir build && cd build
  cmake ..
  make
  ```
- No explicit test framework or test commands found in the project.
- To run a single test, add test framework and CMake test targets as needed.

# Code Style Guidelines

- **File organization:**
  - Header and source files paired (e.g., `aimanager.h` and `aimanager.cpp`).
  - UI files (`.ui`) for Qt interface.
- **Imports:**
  - Use Qt6 modules as found in CMakeLists.txt (Widgets, Network, Svg, Multimedia).
- **Formatting:**
  - Follow C++17 standard.
  - Use consistent indentation and brace style.
- **Types:**
  - Use strong typing and Qt types where appropriate.
- **Naming conventions:**
  - Classes and structs: PascalCase.
  - Variables and functions: camelCase.
  - Constants: UPPER_SNAKE_CASE.
- **Error handling:**
  - Use Qt error reporting mechanisms.
  - Handle exceptions where applicable.

# Cursor and Copilot Rules

- No `.cursor` or `.cursorrules` directory found.
- No `.github/copilot-instructions.md` file found.

This file guides agentic coding agents working in this repository.