#include "thread/count_down_latch.h"
#include <thread>
#include <chrono>

namespace wzq {

CountDownLatch::CountDownLatch(uint32_t count) : count_(count) {}

void CountDownLatch::CountDown() { 
  std::unique_lock<std::mutex> lock(mutex_);
  --count_;
  if (count_ == 0) {
    cv_.notify_all();
  }
}

void CountDownLatch::Await(uint32_t time_ms) { 
   std::unique_lock<std::mutex> lock(mutex_);
   while (count_ > 0) {
     if (time_ms > 0) {
       cv_.wait_for(lock, std::chrono::milliseconds(time_ms));
     } else {
       cv_.wait(lock);
     }
   }
}

uint32_t CountDownLatch::GetCount() const{ 
  std::unique_lock<std::mutex> lock(mutex_);
  return count_; 
}

}  // namespace wzq