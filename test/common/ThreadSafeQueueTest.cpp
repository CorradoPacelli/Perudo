#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>

#include "ThreadSafeQueue.hpp"

class ThreadSafeQueueTest : public ::testing::Test {
protected:
    ThreadSafeQueue<int> queue;
};

TEST_F(ThreadSafeQueueTest, PushAndPopSingleThread) {
    queue.push(42);
    EXPECT_EQ(queue.waitAndPop(), 42);
}

TEST_F(ThreadSafeQueueTest, WaitAndPopBlocksUntilPushed) {


    std::atomic<bool> popped{false};
    int poppedValue = 0;

    std::thread consumer([&]() {
        poppedValue = queue.waitAndPop();
        popped = true;
    });

    // let's wait a little bit to let the thread start and go to sleep
    std::this_thread::sleep_for(std::chrono::milliseconds(50));   
    EXPECT_FALSE(popped.load());

    // Pushing should wake up the condition_variable in waitAndPop
    queue.push(99);
    consumer.join();

    EXPECT_TRUE(popped.load());
    EXPECT_EQ(poppedValue, 99);
}

TEST_F(ThreadSafeQueueTest, ConcurrentPushes) {
    const int numThreads = 10;
    const int itemsPerThread = 100;
    std::vector<std::thread> producers;

    // Spawn multiple threads that push to the queue concurrently
    for (int i = 0; i < numThreads; ++i) {
        producers.emplace_back([&, i]() {
            for (int j = 0; j < itemsPerThread; ++j) {
                queue.push(i * itemsPerThread + j);
            }
        });
    }

    // Wait for all producer threads to finish
    for (auto& t : producers) {
        t.join();
    }

    EXPECT_EQ(queue.size(), 1000);

    // Verify that all items were successfully added (no data races dropped any pushes)
    int count = 0;
    for (int i = 0; i < numThreads * itemsPerThread; ++i) {
        queue.waitAndPop();
        count++;
    }

    EXPECT_EQ(count, numThreads * itemsPerThread);
}
