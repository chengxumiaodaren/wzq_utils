#ifndef __COUNT_DOWN_LATCH__
#define __COUNT_DOWN_LATCH__

#include <condition_variable>
#include <iostream>
#include <mutex>
// #include <stdint.h>
// #include <inttypes.h>

namespace wzq {
class CountDownLatch {
   public:
    CountDownLatch(uint32_t count);

    void CountDown();

    void Await(uint32_t time_ms = 0);

    uint32_t GetCount();

   private:
    std::condition_variable cv_;
    std::mutex mutex_;
    uint32_t count_ = 0;
};
}  // namespace wzq

#endif