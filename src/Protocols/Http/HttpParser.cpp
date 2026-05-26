#include "HttpParser.hpp"
#include "HttpConstants.hpp"
#include <sstream>

namespace Protocols::Http {

std::optional<API::ParsedRequest> HttpParser::feed(const char* data, std::size_t len) {
    buffer_.append(data, len);

    auto sep_pos = buffer_.find(std::string(BODY_SEP));
    if (sep_pos == std::string::npos)
        return std::nullopt; // headers not complete yet

    API::ParsedRequest req;
    std::istringstream stream(buffer_.substr(0, sep_pos));
    std::string line;

    if (!std::getline(stream, line) || !parseRequestLine(line, req))
        return std::nullopt;

    while (std::getline(stream, line) && !line.empty() && line != "\r")
        parseHeader(line, req);

    // Extract body if Content-Length is set
    std::size_t header_end = sep_pos + BODY_SEP.size();
    if (auto it = req.headers.find("Content-Length"); it != req.headers.end()) {
        std::size_t body_len = std::stoul(it->second);
        if (buffer_.size() >= header_end + body_len)
            req.body = buffer_.substr(header_end, body_len);
        else
            return std::nullopt; // body not fully received yet
    }

    return req;
}

void HttpParser::reset() {
    buffer_.clear();
}

bool HttpParser::parseRequestLine(const std::string& line, API::ParsedRequest& req) {
    std::istringstream ss(line);
    return static_cast<bool>(ss >> req.method >> req.uri >> req.version);
}

bool HttpParser::parseHeader(const std::string& line, API::ParsedRequest& req) {
    auto colon = line.find(':');
    if (colon == std::string::npos) return false;

    std::string key   = line.substr(0, colon);
    std::string value = line.substr(colon + 2); // skip ": "
    if (!value.empty() && value.back() == '\r') value.pop_back();
    req.headers[key] = value;
    return true;
}

} // namespace Protocols::Http
