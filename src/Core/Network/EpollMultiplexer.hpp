#pragma once
#include <functional>
#include <sys/epoll.h>
#include <vector>

namespace Core::Network {

// Reactor: multiplexes I/O events via epoll_wait and dispatches callbacks.
class EpollMultiplexer {
  public:
    using EventCallback = std::function<void(int fd, uint32_t events)>;

    explicit EpollMultiplexer(int max_events = 1024);
    ~EpollMultiplexer();

    EpollMultiplexer(const EpollMultiplexer&)            = delete;
    EpollMultiplexer& operator=(const EpollMultiplexer&) = delete;

    void add(int fd, uint32_t events, EventCallback cb);
    void modify(int fd, uint32_t events);
    void remove(int fd);

    // Blocks up to timeout_ms (-1 = forever). Dispatches all ready events.
    void poll(int timeout_ms = -1);

    void stop() { running_ = false; }

  private:
    int epoll_fd_{-1};
    int max_events_;
    bool running_{true};

    std::vector<epoll_event> event_buf_;
    std::unordered_map<int, EventCallback> callbacks_;
};

} // namespace Core::Network
