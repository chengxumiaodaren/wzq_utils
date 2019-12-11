#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>

namespace wzq {
class TimerQueue {
   public:
    struct InternalS {
        std::chrono::time_point<std::chrono::steady_clock> time_point_;
        std::function<void()> func_;
        bool operator<(const InternalS& b) const { return time_point_ > b.time_point_; }
    };

   public:
    void Run() {
        while (running_) {
            std::unique_lock<std::mutex> lock(mutex_);
            if (queue_.empty()) {
                cond_.wait(lock);
                continue;
            }
            auto s = queue_.top();
            auto diff = s.time_point_ - std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() > 0) {
                cond_.wait_for(lock, diff);
                continue;
            } else {
                s.func_();
                queue_.pop();
            }
        }
    }

    int Size() { return queue_.size(); }

    void Stop() {
        running_ = false;
        cond_.notify_all();
    }

    template <typename R, typename P, typename F, typename... Args>
    void AddFuncAfterDuration(const std::chrono::duration<R, P>& time, F&& f, Args&&... args) {
        InternalS s;
        s.time_point_ = std::chrono::steady_clock::now() + time;
        s.func_ = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(s);
        cond_.notify_all();
    }

    TimerQueue() : running_(true) {}

    ~TimerQueue() { running_ = false; }

   private:
    std::priority_queue<InternalS> queue_;
    bool running_ = false;
    std::mutex mutex_;
    std::condition_variable cond_;
};

}  // namespace wzq