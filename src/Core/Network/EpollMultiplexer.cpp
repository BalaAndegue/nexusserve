#include "EpollMultiplexer.hpp"
#include <stdexcept>
#include <cstring>
#include <unistd.h>

namespace Core::Network {

EpollMultiplexer::EpollMultiplexer(int max_events)
    : max_events_(max_events), event_buf_(max_events) {
    epoll_fd_ = ::epoll_create1(EPOLL_CLOEXEC);
    if (epoll_fd_ < 0)
        throw std::runtime_error(std::string("epoll_create1(): ") + strerror(errno));
}

EpollMultiplexer::~EpollMultiplexer() {
    if (epoll_fd_ >= 0) ::close(epoll_fd_);
}

void EpollMultiplexer::add(int fd, uint32_t events, EventCallback cb) {
    callbacks_[fd] = std::move(cb);
    epoll_event ev{};
    ev.events  = events;
    ev.data.fd = fd;
    if (::epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) < 0)
        throw std::runtime_error(std::string("epoll_ctl ADD: ") + strerror(errno));
}

void EpollMultiplexer::modify(int fd, uint32_t events) {
    epoll_event ev{};
    ev.events  = events;
    ev.data.fd = fd;
    ::epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &ev);
}

void EpollMultiplexer::remove(int fd) {
    ::epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr);
    callbacks_.erase(fd);
}

void EpollMultiplexer::poll(int timeout_ms) {
    while (running_) {
        int n = ::epoll_wait(epoll_fd_, event_buf_.data(), max_events_, timeout_ms);
        if (n < 0) {
            if (errno == EINTR) continue;
            break;
        }
        for (int i = 0; i < n; ++i) {
            int fd = event_buf_[i].data.fd;
            if (auto it = callbacks_.find(fd); it != callbacks_.end())
                it->second(fd, event_buf_[i].events);
        }
    }
}

} // namespace Core::Network
