#ifndef __SINGLETON__
#define __SINGLETON__

#include <iostream>
#include <mutex>
#include <thread>

namespace wzq {

template <typename T>
class SingleTon {
 public:
  static T& instance() {
    std::call_once(once_, &SingleTon::init);
    return *value_;
  }

 private:
  SingleTon();
  ~SingleTon();

  SingleTon(const SingleTon&) = delete;
  SingleTon& operator=(const SingleTon&) = delete;

  static void init() { value_ = new T(); }
  static T* value_;

  static std::once_flag once_;
};

}  // namespace wzq

#endif