#pragma once
#include "TaskQueue.hpp"

#include <atomic>
#include <thread>
#include <vector>

namespace Core::Concurrency {

// Fixed-size worker pool. Each worker pulls tasks from the shared queue.
// Adaptive sizing can be added later by spawning/joining workers at runtime.
class ThreadPool {
  public:
    explicit ThreadPool(std::size_t num_threads);
    ~ThreadPool();

    ThreadPool(const ThreadPool&)            = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void submit(TaskQueue::Task task);

    std::size_t pendingTasks() const { return queue_.size(); }

  private:
    void workerLoop();

    TaskQueue queue_;
    std::vector<std::thread> workers_;
    std::atomic<bool> running_{true};
};

} // namespace Core::Concurrency
