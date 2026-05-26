#include <gtest/gtest.h>
#include "Core/Concurrency/ThreadPool.hpp"
#include <atomic>
#include <chrono>

using Core::Concurrency::ThreadPool;

TEST(ThreadPool, ExecutesAllTasks) {
    ThreadPool pool(4);
    std::atomic<int> counter{0};
    const int N = 100;

    for (int i = 0; i < N; ++i)
        pool.submit([&] { counter.fetch_add(1, std::memory_order_relaxed); });

    // Give workers time to drain
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    EXPECT_EQ(counter.load(), N);
}

TEST(ThreadPool, HandlesZeroTasksCleanly) {
    ThreadPool pool(2);
    // Destructor must not hang when no tasks were ever submitted.
}
