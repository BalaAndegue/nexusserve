#pragma once
#include "IProtocolParser.hpp"
#include <string>
#include <unordered_map>

namespace API {

struct HttpResponse {
    int         status_code{200};
    std::string status_text{"OK"};
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

class IRequestHandler {
public:
    virtual ~IRequestHandler() = default;

    // Pure business logic: receives a clean request, returns a response.
    // Must be thread-safe — called concurrently from the thread pool.
    virtual HttpResponse handle(const ParsedRequest& request) = 0;
};

} // namespace API
