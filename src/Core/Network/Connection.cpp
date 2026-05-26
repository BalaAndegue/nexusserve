#include "Connection.hpp"
#include <array>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sstream>

namespace Core::Network {

Connection::Connection(int fd,
                       std::shared_ptr<API::IProtocolParser> parser,
                       std::shared_ptr<API::IRequestHandler> handler)
    : fd_(fd), parser_(std::move(parser)), handler_(std::move(handler)) {
    // Apply a 5-second receive timeout so slow clients don't hold workers.
    struct timeval tv{.tv_sec = 5, .tv_usec = 0};
    ::setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}

Connection::~Connection() {
    if (fd_ >= 0) ::close(fd_);
}

void Connection::handleSync() {
    std::array<char, 4096> buf{};
    while (true) {
        ssize_t n = ::recv(fd_, buf.data(), buf.size(), 0);
        if (n <= 0) return; // client closed or timeout

        auto result = parser_->feed(buf.data(), static_cast<std::size_t>(n));
        if (result) {
            sendResponse(handler_->handle(*result));
            return; // HTTP/1.1 Connection: close — one request per connection
        }
    }
}

void Connection::sendResponse(const API::HttpResponse& resp) {
    std::ostringstream oss;
    oss << "HTTP/1.1 " << resp.status_code << " " << resp.status_text << "\r\n";
    for (auto& [k, v] : resp.headers)
        oss << k << ": " << v << "\r\n";
    oss << "Content-Length: " << resp.body.size() << "\r\n";
    oss << "Connection: close\r\n\r\n";
    oss << resp.body;

    std::string raw = oss.str();
    ::send(fd_, raw.data(), raw.size(), MSG_NOSIGNAL);
}

} // namespace Core::Network
