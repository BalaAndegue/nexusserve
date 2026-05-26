#pragma once
#include "../../API/IProtocolParser.hpp"

namespace Protocols::Http {

// Implements IProtocolParser for HTTP/1.1.
// Accumulates raw bytes, parses request-line + headers + body when complete.
class HttpParser : public API::IProtocolParser {
  public:
    std::optional<API::ParsedRequest> feed(const char* data, std::size_t len) override;
    void reset() override;

  private:
    bool parseRequestLine(const std::string& line, API::ParsedRequest& req);
    bool parseHeader(const std::string& line, API::ParsedRequest& req);

    std::string buffer_;
};

} // namespace Protocols::Http
