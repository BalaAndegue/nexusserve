#include "ThreadPool.hpp"

namespace Core::Concurrency {

ThreadPool::ThreadPool(std::size_t num_threads) {
    workers_.reserve(num_threads);
    for (std::size_t i = 0; i < num_threads; ++i)
        workers_.emplace_back([this] { workerLoop(); });
}

ThreadPool::~ThreadPool() {
    running_ = false;
    queue_.shutdown();
    for (auto& w : workers_)
        if (w.joinable()) w.join();
}

void ThreadPool::submit(TaskQueue::Task task) {
    queue_.push(std::move(task));
}

void ThreadPool::workerLoop() {
    while (running_) {
        auto task = queue_.pop();
        if (!task) break;
        (*task)();
    }
}

} // namespace Core::Concurrency
