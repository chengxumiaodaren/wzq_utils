#ifndef __TIMER__
#define __TIMER__

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <queue>

#include "common/map.h"
#include "thread/thread_pool.h"

namespace wzq {
class TimerQueue {
   public:
    struct InternalS {
        std::chrono::time_point<std::chrono::high_resolution_clock> time_point_;
        std::function<void()> func_;
        int repeated_id;
        bool operator<(const InternalS& b) const { return time_point_ > b.time_point_; }
    };

   public:
    bool Run() {
        bool ret = thread_pool_.Start();
        if (!ret) {
            return false;
        }
        std::thread([this]() { RunLocal(); }).detach();
        return true;
    }

    bool IsAvailable() { return thread_pool_.IsAvailable(); }

    int Size() { return queue_.size(); }

    void Stop() {
        running_.store(false);
        cond_.notify_all();
        thread_pool_.ShutDown();
    }

    template <typename R, typename P, typename F, typename... Args>
    void AddFuncAfterDuration(const std::chrono::duration<R, P>& time, F&& f, Args&&... args) {
        InternalS s;
        s.time_point_ = std::chrono::high_resolution_clock::now() + time;
        s.func_ = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(s);
        cond_.notify_all();
    }

    template <typename F, typename... Args>
    void AddFuncAtTimePoint(const std::chrono::time_point<std::chrono::high_resolution_clock>& time_point, F&& f,
                            Args&&... args) {
        InternalS s;
        s.time_point_ = time_point;
        s.func_ = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(s);
        cond_.notify_all();
    }

    template <typename R, typename P, typename F, typename... Args>
    int AddRepeatedFunc(int repeat_num, const std::chrono::duration<R, P>& time, F&& f, Args&&... args) {
        int id = GetNextRepeatedFuncId();
        repeated_id_state_map_.Emplace(id, RepeatedIdState::kRunning);
        auto tem_func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        AddRepeatedFuncLocal(repeat_num - 1, time, id, std::move(tem_func));
        return id;
    }

    void CancelRepeatedFuncId(int func_id) { repeated_id_state_map_.EraseKey(func_id); }

    int GetNextRepeatedFuncId() { return repeated_func_id_++; }

    TimerQueue() : thread_pool_(wzq::ThreadPool::ThreadPoolConfig{4, 4, 40, std::chrono::seconds(4)}) {
        repeated_func_id_.store(0);
        running_.store(true);
    }

    ~TimerQueue() { Stop(); }

    enum class RepeatedIdState { kInit = 0, kRunning = 1, kStop = 2 };

   private:
    void RunLocal() {
        while (running_.load()) {
            std::unique_lock<std::mutex> lock(mutex_);
            if (queue_.empty()) {
                cond_.wait(lock);
                continue;
            }
            auto s = queue_.top();
            auto diff = s.time_point_ - std::chrono::high_resolution_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() > 0) {
                cond_.wait_for(lock, diff);
                continue;
            } else {
                queue_.pop();
                lock.unlock();
                thread_pool_.Run(std::move(s.func_));
            }
        }
    }

    template <typename R, typename P, typename F>
    void AddRepeatedFuncLocal(int repeat_num, const std::chrono::duration<R, P>& time, int id, F&& f) {
        if (!this->repeated_id_state_map_.IsKeyExist(id)) {
            return;
        }
        InternalS s;
        s.time_point_ = std::chrono::high_resolution_clock::now() + time;
        auto tem_func = std::move(f);
        s.repeated_id = id;
        s.func_ = [this, &tem_func, repeat_num, time, id]() {
            tem_func();
            if (!this->repeated_id_state_map_.IsKeyExist(id) || repeat_num == 0) {
                return;
            }
            AddRepeatedFuncLocal(repeat_num - 1, time, id, std::move(tem_func));
        };
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(s);
        lock.unlock();
        cond_.notify_all();
    }

   private:
    std::priority_queue<InternalS> queue_;
    std::atomic<bool> running_;
    std::mutex mutex_;
    std::condition_variable cond_;

    wzq::ThreadPool thread_pool_;

    std::atomic<int> repeated_func_id_;
    wzq::ThreadSafeMap<int, RepeatedIdState> repeated_id_state_map_;
};

}  // namespace wzq

#endif