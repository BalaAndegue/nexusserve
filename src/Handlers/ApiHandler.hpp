#pragma once
#include "../API/IRequestHandler.hpp"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace Handlers {

// Routes GET/POST requests to registered lambda handlers.
// Unknown routes are forwarded to the fallback handler (e.g. StaticFileHandler).
// Thread-safe for reads — routes are registered before the event loop starts.
class ApiHandler : public API::IRequestHandler {
  public:
    using RouteHandler = std::function<API::HttpResponse(const API::ParsedRequest&)>;

    explicit ApiHandler(std::shared_ptr<API::IRequestHandler> fallback = nullptr);

    void addRoute(const std::string& method, const std::string& path, RouteHandler h);

    API::HttpResponse handle(const API::ParsedRequest& req) override;

  private:
    // Key: "METHOD /path"
    std::unordered_map<std::string, RouteHandler> routes_;
    std::shared_ptr<API::IRequestHandler> fallback_;
};

} // namespace Handlers
