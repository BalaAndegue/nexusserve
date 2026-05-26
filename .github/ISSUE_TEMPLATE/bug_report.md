---
name: Bug Report
about: Report a reproducible defect
title: "[BUG] "
labels: bug
assignees: ''
---

## Environment
- OS / kernel: <!-- e.g. Ubuntu 22.04, Linux 5.15 -->
- Compiler: <!-- e.g. GCC 12.3 -->
- Build type: <!-- Debug / Release -->
- Commit / tag: <!-- git rev-parse --short HEAD -->

## Description
<!-- A clear, concise description of the bug. -->

## Steps to Reproduce
```bash
# Minimal commands to trigger the bug
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/server &
curl http://localhost:8080/...
```

## Expected Behavior
<!-- What should happen. -->

## Actual Behavior
<!-- What actually happens — include full output, ASan/UBSan reports, etc. -->

## Additional Context
<!-- Logs, packet captures, perf data — anything relevant. -->
