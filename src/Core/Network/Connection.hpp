#pragma once
#include "../../API/IProtocolParser.hpp"
#include "../../API/IRequestHandler.hpp"
#include <memory>
#include <string>

namespace Core::Network {

// Owns one client file descriptor and its parse/handle pipeline.
// handleSync() blocks until the request is complete — designed to be called
// from a thread-pool worker so the event loop stays free for accept().
class Connection {
public:
    Connection(int fd,
               std::shared_ptr<API::IProtocolParser> parser,
               std::shared_ptr<API::IRequestHandler> handler);
    ~Connection();

    Connection(const Connection&)            = delete;
    Connection& operator=(const Connection&) = delete;

    // Blocking: reads → parses → handles → writes → returns.
    // The file descriptor is closed in the destructor.
    void handleSync();

private:
    void sendResponse(const API::HttpResponse& resp);

    int  fd_;
    std::shared_ptr<API::IProtocolParser> parser_;
    std::shared_ptr<API::IRequestHandler> handler_;
};

} // namespace Core::Network
