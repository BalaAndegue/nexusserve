#pragma once
#include "../API/IRequestHandler.hpp"
#include <string>

namespace Handlers {

// Serves files from a root directory. Stateless — safe for concurrent use.
class StaticFileHandler : public API::IRequestHandler {
public:
    explicit StaticFileHandler(std::string root);

    API::HttpResponse handle(const API::ParsedRequest& req) override;

private:
    API::HttpResponse serveFile(const std::string& path);
    static std::string mimeType(const std::string& path);

    std::string root_;
};

} // namespace Handlers
