#include "ApiHandler.hpp"

namespace Handlers {

ApiHandler::ApiHandler(std::shared_ptr<API::IRequestHandler> fallback)
    : fallback_(std::move(fallback)) {}

void ApiHandler::addRoute(const std::string& method,
                          const std::string& path,
                          RouteHandler h) {
    routes_[method + " " + path] = std::move(h);
}

API::HttpResponse ApiHandler::handle(const API::ParsedRequest& req) {
    auto key = req.method + " " + req.uri;
    if (auto it = routes_.find(key); it != routes_.end())
        return it->second(req);

    if (fallback_)
        return fallback_->handle(req);

    return {404, "Not Found",
            {{"Content-Type", "application/json"}},
            "{\"error\":\"route not found\"}"};
}

} // namespace Handlers
