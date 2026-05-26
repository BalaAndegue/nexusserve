#pragma once
#include <optional>
#include <string>
#include <unordered_map>

namespace API {

struct ParsedRequest {
    std::string method;
    std::string uri;
    std::string version;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

class IProtocolParser {
  public:
    virtual ~IProtocolParser() = default;

    // Returns a fully parsed request or nullopt if more data is needed.
    virtual std::optional<ParsedRequest> feed(const char* data, std::size_t len) = 0;
    virtual void reset()                                                         = 0;
};

} // namespace API
