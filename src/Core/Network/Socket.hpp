#pragma once
#include <cstdint>

namespace Core::Network {

// RAII wrapper for a non-blocking TCP socket file descriptor.
class Socket {
  public:
    explicit Socket(uint16_t port);
    ~Socket();

    Socket(const Socket&)            = delete;
    Socket& operator=(const Socket&) = delete;
    Socket(Socket&& other) noexcept;

    int fd() const { return fd_; }
    bool valid() const { return fd_ >= 0; }

  private:
    void setNonBlocking();
    void setReuseAddr();

    int fd_{-1};
};

} // namespace Core::Network
