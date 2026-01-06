AGENTS.md

Purpose

This file provides repository-specific guidance for automated coding agents working in this C++/CMake project. It replaces generic multi-language advice with concrete commands, conventions and a testing strategy that matches the project's layout and tooling.

Repository summary

- Minimal C++ project using CMake. Sources live under `src/` with a small library in `src/tetris/` and a binary `src/main.cpp`.
- Build artifacts (current) are placed in `build/` (out-of-source build recommended).
- `.clang-format` and `.clang-tidy` are present at repo root; prefer these for formatting and static analysis.

Quick references

- Project root: `.`
- Library sources: `src/tetris/` (e.g. `src/tetris/tetris.h`, `src/tetris/tetris.cpp`)
- Binary: `src/main.cpp`
- Recommended test directory (create when adding tests): `tests/` (see Test Strategy below)

Build & test (recommended local workflow)

1. Create an out-of-source build directory and configure (Debug by default):
   - `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug`
2. Build targets (parallel):
   - `cmake --build build -- -j` (or `cmake --build build -- -jN`)
3. Run all tests (CTest must be enabled in CMakeLists):
   - `ctest --test-dir build -j` or `ctest -C Debug --test-dir build -V`
4. Run a single test by name/regex using CTest:
   - `ctest --test-dir build -R "^MyTestName$" -V`
   - For partial matches: `ctest --test-dir build -R "MyTest" -V`
5. Run a single test executable directly (when debugging):
   - `./build/tests/my_test` (executable path depends on your `CMakeLists.txt`)

If you prefer `make` style builds (older CMake generators):
- `mkdir -p build && cd build && cmake .. && make -j` then `ctest -V`.

Enabling clang-tidy from CMake (suggested)
- Configure with: `cmake -S . -B build -DCMAKE_CXX_CLANG_TIDY="clang-tidy;-checks=*"`
- Or integrate via `run-clang-tidy` when available: `run-clang-tidy -p build`

Formatting & linting

- Format code with the repo's `.clang-format`:
  - `clang-format -i $(git ls-files '*.cpp' '*.h' | tr '\n' ' ')` (or use your editor integration)
  - Or format changed files only: `git diff --name-only --diff-filter=ACMRTUXB HEAD | grep -E '\.(cpp|cc|cxx|h|hpp)$' | xargs clang-format -i`
- Static analysis with `clang-tidy` (see above) and optionally `cppcheck`:
  - `cppcheck --enable=warning,style,performance --std=c++17 --project=build/compile_commands.json`
- Run `clang-tidy`/`cppcheck` before larger merges; keep fixes minimal and explain changes.

Test strategy (project-specific)

This repository's testing strategy is executable-based tests placed under a `tests/` directory. Each test is a small standalone executable that is built by CMake and registered with CTest.

Why executables?
- Simple to implement without pulling heavy frameworks.
- Easy to run individually and attach a debugger.
- Fits the project's CMake workflow and is compatible with CI.

How to add a test (recommended minimal example)

1. Create `tests/CMakeLists.txt` alongside `tests/`.
2. Add a simple test executable and register it with CTest. Example snippet to add to your top-level `CMakeLists.txt`:

   add_subdirectory(tests)

   # In tests/CMakeLists.txt (example)
   cmake_minimum_required(VERSION 3.14)
   project(tetris_tests)

   enable_testing()

   # Example: small harness test using only C++ stdlib
   add_executable(test_greet test_greet.cpp)
   target_include_directories(test_greet PRIVATE ${PROJECT_SOURCE_DIR}/src)
   target_compile_features(test_greet PRIVATE cxx_std_17)
   add_test(NAME greet_test COMMAND test_greet)

3. Example `tests/test_greet.cpp` (very small):

   #include <iostream>
   #include "tetris/tetris.h"

   int main() {
       Tetris t;
       auto s = t.greet();
       if (s != "Tetris library ready") return 1;
       std::cout << "OK: " << s << std::endl;
       return 0;
   }

4. Build and run the test:
   - `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug`
   - `cmake --build build --target test || cmake --build build -- -j` (some generators create a `test` target)
   - `ctest --test-dir build -V` or `ctest --test-dir build -R '^greet_test$' -V`

Single-test local debug steps
- Build only tests: `cmake --build build --target test` (if target exists) or `cmake --build build -- -j` then run `./build/tests/test_greet`.
- Attach a debugger directly to `build/tests/test_greet`.

CMake tips for tests
- Use `target_compile_features(target PRIVATE cxx_std_17)` or higher.
- Prefer `target_include_directories(... PRIVATE ...)` over global include paths.
- Keep tests small and fast; avoid network or long sleeps.
- Use `add_test(NAME <name> COMMAND <executable> [args])` to register tests.
- Use `gtest` or `Catch2` if you want richer assertions—prefer FetchContent to vendorizing.

Code style & conventions (C++ specific)

Formatting & whitespace
- Use repository `.clang-format`; run `clang-format -i` before pushing.
- Existing files use tabs for indentation. Preserve tab-style where present. New files may follow the `.clang-format` rules.

Includes and header guards
- Use `#pragma once` for header guards (project already uses it).
- Order includes as:
  1. Corresponding header (if .cpp has a header)
  2. C / C++ standard library headers
  3. Third-party library headers
  4. Project headers (relative path)
- Keep one blank line between groups.

Naming
- Types / classes: `PascalCase` (e.g., `Tetris`).
- Functions / methods: `snake_case` or `camelCase` — follow existing style; `greet()` is used in code.
- Files and directories: lower-case, use `snake_case` for files (e.g., `tetris.cpp`, `tetris.h`) and directories as `tetris/`.

API & header design
- Keep headers minimal and focused; prefer forward declarations where possible.
- Use `const &` for heavy parameter passing and `std::string_view` for read-only string parameters where convenient.
- Document public APIs with brief comments above declarations.

Types and error handling
- Prefer RAII for resource management; avoid naked new/delete.
- Prefer returning `bool`/`enum` or throwing exceptions for error cases depending on API design; document behavior.
- For library code, prefer returning error codes or `std::optional`/`expected`-style types for recoverable errors.
- Add context to exceptions (where thrown) and catch at top-level `main` to print user-friendly messages.

Tests and determinism
- Tests must be deterministic and isolated.
- Avoid reliance on wall-clock time or external systems unless explicitly marked integration tests.
- Use small fixtures within `tests/` or common `tests/util/` helpers.

Logging & debug
- Use `std::cerr` or a minimal logging utility for errors and warnings; keep debug output gated behind a verbosity flag.

Editing & commits
- Before writing code changes, run `clang-format` and `clang-tidy` locally on changed files.
- Do not create commits automatically. If asked to create a commit, keep it focused and include a short explanatory message.

Agent runtime behavior (repo rules)

- Before any write/destructive operation, provide a 1–2 sentence preamble describing intended actions.
- When running shell commands, use the CMake and clang tools described above.
- When adding tests, create `tests/` with a `CMakeLists.txt` and register executables with `add_test`.
- Validate changes by building just the affected targets and running the smallest test(s) that exercise the modification.

Cursor / Copilot rules

- No `.cursor/` rules or `.cursorrules` were found when this file was created.
- No `.github/copilot-instructions.md` was found when this file was created.
- If any of these files are added later, update this `AGENTS.md` to include and follow them.

Security

- Never add secrets or credentials to the repo. If secrets are found, stop and notify the human immediately.

Last generated: (automatically created by an agent). Update as the project acquires new tooling or conventions.
