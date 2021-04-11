#ifndef __WAIT_UTIL_H_
#define __WAIT_UTIL_H_

#include <chrono>
#include <thread>

namespace chrono = std::chrono;
using namespace std::chrono_literals;

template <typename TASK_TYPE>
inline void WaitByTask(TASK_TYPE task, const size_t numAttempts = 4,
                       const chrono::milliseconds attemptSleepTime = 250ms)
{
    for (size_t i = 0; i < numAttempts; i++)
    {
        if (task())
        {
            break;
        }

        std::this_thread::sleep_for(attemptSleepTime);
    }
}

#endif  // __WAIT_UTIL_H_
