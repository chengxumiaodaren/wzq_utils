#ifndef __NONCOPYABLE__
#define __NONCOPYABLE__

#include <iostream>

namespace wzq {

class NonCopyAble {
   public:
    NonCopyAble(const NonCopyAble&) = delete;
    NonCopyAble& operator=(const NonCopyAble&) = delete;

   protected: // 禁止多态调用时delete 基类指针
    NonCopyAble() = default;
    ~NonCopyAble() = default;
};

}  // namespace wzq

#endif
