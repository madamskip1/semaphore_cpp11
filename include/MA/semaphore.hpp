#pragma once

#include <cassert>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <mutex>

namespace MA {

template <std::ptrdiff_t LeastMaxValue = std::numeric_limits<std::ptrdiff_t>::max()>
class counting_semaphore {
public:
    explicit counting_semaphore(const std::ptrdiff_t desired)
        : counter { desired }
    {
        assert(desired >= 0 && desired <= LeastMaxValue);
    }

    counting_semaphore(const counting_semaphore&) = delete;
    counting_semaphore& operator=(const counting_semaphore&) = delete;

    void release(const std::ptrdiff_t update = 1)
    {
        {
            std::lock_guard<std::mutex> lock { mutex };
            assert(update >= 0 && update <= (LeastMaxValue - counter));

            counter += update;
        }

        for (auto i = std::ptrdiff_t { 0 }; i < update; ++i) {
            conditionVariable.notify_one();
        }
    }

    void acquire()
    {
        std::unique_lock<std::mutex> lock { mutex };

        conditionVariable.wait(lock, [this]() { return counter > 0; });
        --counter;
    }

    bool try_acquire() noexcept
    {
        std::lock_guard<std::mutex> lock { mutex };

        if (counter == 0) {
            return false;
        }

        --counter;
        return true;
    }

    template <class Rep, class Period>
    bool try_acquire_for(const std::chrono::duration<Rep, Period>& rel_time)
    {
        std::unique_lock<std::mutex> lock { mutex };

        auto acquired = conditionVariable.wait_for(lock, rel_time, [this]() { return counter > 0; });
        if (!acquired) {
            return false;
        }

        --counter;
        return true;
    }

    template <class Clock, class Duration>
    bool try_acquire_until(const std::chrono::time_point<Clock, Duration>& abs_time)
    {
        std::unique_lock<std::mutex> lock { mutex };

        auto acquired = conditionVariable.wait_until(lock, abs_time, [this]() { return counter > 0; });
        if (!acquired) {
            return false;
        }

        --counter;
        return true;
    }

    static constexpr std::ptrdiff_t max() noexcept
    {
        return LeastMaxValue;
    }

private:
    std::ptrdiff_t counter;
    std::mutex mutex;
    std::condition_variable conditionVariable;
};

using binary_semaphore = counting_semaphore<1>;

}
