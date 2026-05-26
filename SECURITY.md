# Security Policy

## Supported Versions

| Version | Supported |
|---------|-----------|
| main    | ✅ Yes     |

## Reporting a Vulnerability

**Do not open a public GitHub issue for security vulnerabilities.**

Please email **balaandeguefrancoislionnel@gmail.com** with:

1. A clear description of the vulnerability
2. Steps to reproduce (ideally a minimal `curl` / `nc` command)
3. The potential impact
4. Any suggested fix (optional)

You will receive an acknowledgement within **48 hours** and a resolution
timeline within **7 days**.

## Known Security Considerations

- **Path traversal**: `StaticFileHandler` uses `std::filesystem::weakly_canonical`
  to reject paths that escape the document root. Do not disable this check.
- **Input validation**: The HTTP parser rejects requests with missing request-line
  fields; no arbitrary memory is exposed.
- **Port binding**: The server binds to `0.0.0.0` by default. In production,
  place it behind a reverse proxy (Nginx / Caddy) that handles TLS.
- **No TLS**: This implementation does not provide HTTPS. Always terminate TLS
  at the edge.
