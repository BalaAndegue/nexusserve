#pragma once
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

namespace Core::Concurrency {

// Lock-based MPMC queue. Workers block on pop(); producers call push().
class TaskQueue {
public:
    using Task = std::function<void()>;

    void push(Task t) {
        {
            std::lock_guard lk(mu_);
            queue_.push(std::move(t));
        }
        cv_.notify_one();
    }

    // Blocks until a task is available or the queue is shut down.
    std::optional<Task> pop() {
        std::unique_lock lk(mu_);
        cv_.wait(lk, [this] { return !queue_.empty() || done_; });
        if (queue_.empty()) return std::nullopt;
        Task t = std::move(queue_.front());
        queue_.pop();
        return t;
    }

    void shutdown() {
        { std::lock_guard lk(mu_); done_ = true; }
        cv_.notify_all();
    }

    std::size_t size() const {
        std::lock_guard lk(mu_);
        return queue_.size();
    }

private:
    std::queue<Task>        queue_;
    mutable std::mutex      mu_;
    std::condition_variable cv_;
    bool                    done_{false};
};

} // namespace Core::Concurrency
