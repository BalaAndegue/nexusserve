# Contributing

Thank you for taking the time to contribute. This document explains the
workflow and standards expected for all contributions.

## Development Setup

```bash
git clone https://github.com/<your-handle>/my_http_server.git
cd my_http_server

cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug   # Debug = ASan + UBSan enabled
cmake --build build --parallel $(nproc)
cd build && ctest --output-on-failure
```

## Branch Naming

| Type      | Pattern               | Example                   |
|-----------|-----------------------|---------------------------|
| Feature   | `feat/<short-desc>`   | `feat/websocket-upgrade`  |
| Bug fix   | `fix/<short-desc>`    | `fix/parser-boundary`     |
| Refactor  | `refactor/<short-desc>`| `refactor/epoll-wrapper` |
| Docs      | `docs/<short-desc>`   | `docs/contributing`       |

## Commit Messages

Follow [Conventional Commits](https://www.conventionalcommits.org/):

```
<type>(<scope>): <short imperative description>

[optional body — explain WHY, not WHAT]

[optional footer — breaking change, closes #issue]
```

**Types**: `feat`, `fix`, `refactor`, `perf`, `test`, `docs`, `ci`, `chore`

**Scopes**: `core/network`, `core/concurrency`, `protocols/http`, `handlers`,
`utils`, `build`, `api`

Example:
```
feat(handlers): add WebSocket upgrade handler

Implements RFC 6455 handshake. The parser interface (IProtocolParser)
required no changes — only a new Protocols/Ws/ directory was added.

Closes #42
```

## Code Standards

- **Standard**: C++20
- **Formatting**: `.clang-format` (run `clang-format -i` before committing)
- **No comments** unless the *why* is non-obvious
- **No raw owning pointers** — use `unique_ptr` / `shared_ptr`
- **Thread safety**: document which methods are thread-safe in the header

## Adding a Test

Place new tests under `tests/`. Use Google Test:

```cpp
#include <gtest/gtest.h>
#include "YourHeader.hpp"

TEST(SuiteName, DescriptiveName) {
    // Arrange
    // Act
    // Assert
}
```

Run `cmake --build build && cd build && ctest` to verify.

## Performance Changes

If your PR touches the hot path (accept loop, parser, thread pool), include
before/after `ab` numbers in the PR description:

```bash
ab -n 10000 -c 100 http://127.0.0.1:8080/api/health
```

## Pull Request Checklist

See `.github/PULL_REQUEST_TEMPLATE.md` — it is auto-populated when you open a PR.
