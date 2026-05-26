#include "Utils/ConfigManager.hpp"
#include "Utils/Logger.hpp"
#include "Core/Network/Socket.hpp"
#include "Core/Network/EpollMultiplexer.hpp"
#include "Core/Network/Connection.hpp"
#include "Core/Concurrency/ThreadPool.hpp"
#include "Protocols/Http/HttpParser.hpp"
#include "Handlers/StaticFileHandler.hpp"
#include "Handlers/ApiHandler.hpp"

#include <csignal>
#include <netinet/in.h>
#include <sys/socket.h>
#include <memory>
#include <thread>

static volatile bool g_running = true;

int main() {
    auto cfg = Utils::ConfigManager::load("config.json");

    if      (cfg.log_level == "DEBUG") Utils::Logger::instance().setLevel(Utils::LogLevel::DEBUG);
    else if (cfg.log_level == "WARN")  Utils::Logger::instance().setLevel(Utils::LogLevel::WARN);
    else if (cfg.log_level == "ERROR") Utils::Logger::instance().setLevel(Utils::LogLevel::ERROR);

    std::signal(SIGINT,  [](int) { g_running = false; });
    std::signal(SIGTERM, [](int) { g_running = false; });

    int thread_count = cfg.thread_pool_size > 0
        ? cfg.thread_pool_size
        : static_cast<int>(std::thread::hardware_concurrency());

    LOG_INFO("Starting server on port ", cfg.port, " with ", thread_count, " worker threads");

    // Handler pipeline: API routes → fallback to static files
    auto staticHandler = std::make_shared<Handlers::StaticFileHandler>(cfg.public_root);
    auto apiHandler    = std::make_shared<Handlers::ApiHandler>(staticHandler);

    apiHandler->addRoute("GET", "/api/health", [](const API::ParsedRequest&) -> API::HttpResponse {
        return {200, "OK", {{"Content-Type", "application/json"}}, "{\"status\":\"ok\"}"};
    });

    Core::Concurrency::ThreadPool   pool(thread_count);
    Core::Network::Socket           listen_sock(cfg.port);
    Core::Network::EpollMultiplexer epoll;

    // Reactor: the event loop owns only the listening socket.
    // Each accepted client fd is immediately handed off to a worker thread.
    // Workers own the full connection lifecycle — no shared mutable state.
    epoll.add(listen_sock.fd(), EPOLLIN, [&](int, uint32_t) {
        sockaddr_in client_addr{};
        socklen_t   len       = sizeof(client_addr);
        int         client_fd = ::accept4(listen_sock.fd(),
                                          reinterpret_cast<sockaddr*>(&client_addr),
                                          &len, SOCK_CLOEXEC);
        if (client_fd < 0) return;

        LOG_DEBUG("New connection fd=", client_fd);

        // Capture shared_ptr by value — each worker gets its own ref count.
        pool.submit([client_fd, apiHandler] {
            auto parser = std::make_shared<Protocols::Http::HttpParser>();
            Core::Network::Connection conn(client_fd, parser, apiHandler);
            conn.handleSync();
        });
    });

    LOG_INFO("Server ready — http://0.0.0.0:", cfg.port);

    while (g_running)
        epoll.poll(500);

    LOG_INFO("Shutdown signal received, stopping.");
    return 0;
}
