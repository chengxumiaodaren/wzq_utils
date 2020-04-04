#ifndef __THREAD_SAFE_MAP__
#define __THREAD_SAFE_MAP__

#include <map>
#include <mutex>

namespace wzq {
// thread safe map
template <typename K, typename V>
class ThreadSafeMap {
   public:
    void Emplace(const K& key, const V& v) {
        std::unique_lock<std::mutex> lock(mutex_);
        map_[key] = v;
    }

    void Emplace(const K& key, V&& v) {
        std::unique_lock<std::mutex> lock(mutex_);
        map_[key] = std::move(v);
    }

    void EraseKey(const K& key) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (map_.find(key) != map_.end()) {
            map_.erase(key);
        }
    }

    bool GetValueFromKey(const K& key, V& value) {
        std::unique_lock<std::mutex> l(mutex_);
        if (map_.find(key) != map_.end()) {
            value = map_[key];
            return true;
        }
        return false;
    }

    bool IsKeyExist(const K& key) {
        std::unique_lock<std::mutex> l(mutex_);
        return map_.find(key) != map_.end();
    }

    std::size_t Size() {
        std::unique_lock<std::mutex> l(mutex_);
        return map_.size();
    }

   private:
    std::map<K, V> map_;
    std::mutex mutex_;
};

}  // namespace wzq

#endif