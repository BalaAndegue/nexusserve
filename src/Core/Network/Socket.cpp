#include "Socket.hpp"

#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

namespace Core::Network {

Socket::Socket(uint16_t port) {
    fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0)
        throw std::runtime_error(std::string("socket(): ") + strerror(errno));

    setReuseAddr();
    setNonBlocking();

    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(port);

    if (::bind(fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
        throw std::runtime_error(std::string("bind(): ") + strerror(errno));

    if (::listen(fd_, SOMAXCONN) < 0)
        throw std::runtime_error(std::string("listen(): ") + strerror(errno));
}

Socket::~Socket() {
    if (fd_ >= 0)
        ::close(fd_);
}

Socket::Socket(Socket&& other) noexcept
    : fd_(other.fd_) {
    other.fd_ = -1;
}

void Socket::setNonBlocking() {
    int flags = ::fcntl(fd_, F_GETFL, 0);
    if (flags < 0 || ::fcntl(fd_, F_SETFL, flags | O_NONBLOCK) < 0)
        throw std::runtime_error(std::string("fcntl(): ") + strerror(errno));
}

void Socket::setReuseAddr() {
    int opt = 1;
    ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

} // namespace Core::Network
