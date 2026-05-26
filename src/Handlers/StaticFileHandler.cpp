#include "StaticFileHandler.hpp"
#include "../Protocols/Http/HttpConstants.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace Protocols::Http;

namespace Handlers {

StaticFileHandler::StaticFileHandler(std::string root) : root_(std::move(root)) {}

API::HttpResponse StaticFileHandler::handle(const API::ParsedRequest& req) {
    if (req.method != "GET" && req.method != "HEAD") {
        return {405, "Method Not Allowed", {{"Allow", "GET, HEAD"}}, ""};
    }

    std::string uri = req.uri;
    if (uri == "/") uri = "/index.html";

    // Prevent path traversal
    fs::path target = fs::weakly_canonical(fs::path(root_) / uri.substr(1));
    fs::path root   = fs::weakly_canonical(root_);
    if (target.string().find(root.string()) != 0)
        return {403, "Forbidden", {}, ""};

    return serveFile(target.string());
}

API::HttpResponse StaticFileHandler::serveFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file)
        return {STATUS_NOT_FOUND, "Not Found", {}, "<h1>404 Not Found</h1>"};

    std::ostringstream ss;
    ss << file.rdbuf();
    std::string body = ss.str();

    return {STATUS_OK, "OK",
            {{"Content-Type", mimeType(path)}},
            std::move(body)};
}

std::string StaticFileHandler::mimeType(const std::string& path) {
    auto ext = fs::path(path).extension().string();
    if (ext == ".html" || ext == ".htm") return "text/html; charset=utf-8";
    if (ext == ".css")                   return "text/css";
    if (ext == ".js")                    return "application/javascript";
    if (ext == ".json")                  return "application/json";
    if (ext == ".png")                   return "image/png";
    if (ext == ".jpg" || ext == ".jpeg") return "image/jpeg";
    if (ext == ".svg")                   return "image/svg+xml";
    return "application/octet-stream";
}

} // namespace Handlers
