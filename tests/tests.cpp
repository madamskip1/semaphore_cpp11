#include <chrono>
#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include <MA/semaphore.hpp>

TEST(SemaphoreTest, AcquireImmediately)
{
    MA::counting_semaphore<1> semaphore(1);

    semaphore.acquire();

    // if won't get here, test fails
}

TEST(SemaphoreTest, AcquireAfterRelease)
{
    MA::counting_semaphore<1> semaphore(0);

    semaphore.release();
    semaphore.acquire();

    // if won't get here, test fails
}

TEST(SemaphoreTest, AcquireAfterAnotherThreadReleases)
{
    MA::counting_semaphore<1> semaphore(0);

    std::thread acquireThread([&]
    {
        semaphore.acquire();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    semaphore.release();
    acquireThread.join();

    // if won't get here, test fails
}

TEST(SemaphoreTest, TryAcquireWhenSemaphoreLocked)
{
    MA::counting_semaphore<1> semaphore(0);

    EXPECT_FALSE(semaphore.try_acquire());
}

TEST(SemaphoreTest, TryAcquireWhenSemaphoreUnlocked)
{
    MA::counting_semaphore<1> semaphore(1);

    EXPECT_TRUE(semaphore.try_acquire());
}

TEST(SemaphoreTest, TryAcquireForWhenSemaphoreLocked)
{
    MA::counting_semaphore<1> semaphore(0);

    EXPECT_FALSE(semaphore.try_acquire_for(std::chrono::milliseconds(10)));
}

TEST(SemaphoreTest, TryAcquireForWhenSemaphoreUnlocked)
{
    MA::counting_semaphore<1> semaphore(1);

    EXPECT_TRUE(semaphore.try_acquire_for(std::chrono::milliseconds(10)));
}

TEST(SemaphoreTest, ReleaseDuringTryAcquireFor)
{
    MA::counting_semaphore<1> semaphore(0);
    auto acquired = false;

    auto acquireThread = std::thread([&]
    {
        acquired = semaphore.try_acquire_for(std::chrono::milliseconds(100));
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    semaphore.release();
    acquireThread.join();

    EXPECT_TRUE(acquired);
}

TEST(SemaphoreTest, TryAcquireWaitsCloseToTimeout)
{
    MA::counting_semaphore<1> semaphore(0);

    auto start = std::chrono::steady_clock::now();
    bool acquired = semaphore.try_acquire_for(std::chrono::milliseconds(100));
    auto elapsed = std::chrono::steady_clock::now() - start;

    EXPECT_FALSE(acquired);
    EXPECT_GE(elapsed, std::chrono::milliseconds(90)); // should wait close to 100ms
}

TEST(SemaphoreTest, TryAcquireUntilWhenSemaphoreLocked)
{
    MA::counting_semaphore<1> semaphore(0);

    EXPECT_FALSE(semaphore.try_acquire_until(std::chrono::steady_clock::now()));
}

TEST(SemaphoreTest, TryAcquireUntilWhenSemaphoreUnlocked)
{
    MA::counting_semaphore<1> semaphore(1);

    EXPECT_TRUE(semaphore.try_acquire_until(std::chrono::steady_clock::now()));
}

TEST(SemaphoreTest, ReleaseDuringTryAcquireUntil)
{
    MA::counting_semaphore<1> semaphore(0);
    auto acquired = false;
    auto acquireThread = std::thread([&]
    {
        acquired = semaphore.try_acquire_until(std::chrono::steady_clock::now() + std::chrono::milliseconds(100));
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    semaphore.release();
    acquireThread.join();

    EXPECT_TRUE(acquired);
}

TEST(SemaphoreTest, TryAcquireUntilWaitsCloseToTimeout)
{
    MA::counting_semaphore<1> semaphore(0);

    auto start = std::chrono::steady_clock::now();
    bool acquired = semaphore.try_acquire_until(std::chrono::steady_clock::now() + std::chrono::milliseconds(100));
    auto elapsed = std::chrono::steady_clock::now() - start;

    EXPECT_FALSE(acquired);
    EXPECT_GE(elapsed, std::chrono::milliseconds(90)); // should wait close to 100ms
}

TEST(SemaphoreTest, MultiThreadAcquireRelease)
{
    MA::counting_semaphore<3> semaphore(1);
    auto counter = 0;

    auto worker = [&](int)
    {
        semaphore.acquire();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        counter++;
        semaphore.release();
    };

    std::vector<std::thread> threads;
    for (auto i = 0; i < 3; ++i)
    {
        threads.emplace_back(worker, i);
    }

    for (auto& t : threads)
    {
        t.join();
    }

    EXPECT_EQ(counter, 3);
}

TEST(SemaphoreTest, ThreadWaitsOnAcquire)
{
    MA::counting_semaphore<1> semaphore(0);
    auto threadCreated = false;
    auto threadDone = false;

    std::thread t([&]
    {
        threadCreated = true;
        semaphore.acquire();
        threadDone = true;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_TRUE(threadCreated);
    EXPECT_FALSE(threadDone); // must be waiting

    semaphore.release();
    t.join();

    EXPECT_TRUE(threadDone);
}

TEST(SemaphoreTest, PingPong)
{
    MA::counting_semaphore<1> semaphorePing(1); // ping starts with permit
    MA::counting_semaphore<1> semaphorePong(0); // pong waits initially

    const int PINGPONG_TURNS = 1000;
    int counter = 0;

    std::thread pingThread([&]
    {
        for (int i = 0; i < PINGPONG_TURNS; ++i)
        {
            semaphorePing.acquire();
            ++counter;
            semaphorePong.release();
        }
    });

    std::thread pongThread([&]
    {
        for (int i = 0; i < PINGPONG_TURNS; ++i)
        {
            semaphorePong.acquire();
            ++counter;
            semaphorePing.release();
        }
    });

    pingThread.join();
    pongThread.join();

    EXPECT_EQ(counter, 2 * PINGPONG_TURNS);
}

TEST(SemaphoreTest, ReleaseAboveMaxValue)
{
    MA::counting_semaphore<1> semaphore(1);

    EXPECT_DEATH(semaphore.release(), "");
}

TEST(SemaphoreTest, ReleaseNegativeUpdateBelowZero)
{
    MA::counting_semaphore<1> semaphore(1);

    EXPECT_DEATH(semaphore.release(-1), "");
}

TEST(SemaphoreTest, ConstructDesiredAboveMaxValue)
{
    EXPECT_DEATH(MA::counting_semaphore<1>(2), "");
}

TEST(SemaphoreTest, ConstructDesiredBelowZero)
{
    EXPECT_DEATH(MA::counting_semaphore<1>(-1), "");
}

TEST(SemaphoreTest, GetMaxValue)
{
    EXPECT_EQ(MA::counting_semaphore<1>::max(), 1);
}

TEST(SemaphoreTest, BinarySemaphoreMaxValue)
{
    EXPECT_EQ(MA::binary_semaphore::max(), 1);
}