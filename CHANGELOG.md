# Changelog

All notable changes to this project are documented here.  
Format: [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) — versioning: [SemVer](https://semver.org/).

---

## [Unreleased]

### Planned
- HTTP/1.1 persistent connections (`Connection: keep-alive`)
- `sendfile(2)` zero-copy for static file delivery
- WebSocket upgrade via `Protocols/Ws/`
- TLS termination via mbedTLS or BoringSSL
- Prometheus `/metrics` endpoint

---

## [1.0.0] — 2025-05-26

### Added
- **Reactor event loop** — single `epoll_wait` thread accepts all connections
- **Adaptive thread pool** — auto-sized to `std::thread::hardware_concurrency()`
- **HTTP/1.1 parser** — request-line, headers, body via `IProtocolParser`
- **Static file handler** — path-traversal protection via `std::filesystem`
- **API router** — `ApiHandler` with fallback chain to static files
- **Thread-safe logger** — singleton, zero external dependencies
- **Cloud-native config** — `config.json` + ENV-VAR overrides (`SERVER_PORT`, `LOG_LEVEL`, …)
- **Google Test suite** — 5 tests for `HttpParser` and `ThreadPool`
- **GitHub Actions CI** — build + test matrix (Ubuntu 22.04 / 24.04, Debug / Release)
- **AddressSanitizer + UBSan** enabled automatically in Debug builds

### Architecture
- Abstract `IProtocolParser` / `IRequestHandler` interfaces enable protocol swapping
  without touching the network engine
- Stateless handlers allow horizontal scaling behind any load balancer

[Unreleased]: https://github.com/<your-handle>/my_http_server/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/<your-handle>/my_http_server/releases/tag/v1.0.0
