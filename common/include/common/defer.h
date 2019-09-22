#ifndef __DEFER__
#define __DEFER__

#include "common/noncopyable.h"

#include <functional>
#include <iostream>

// reference https://github.com/loveyacper/ananas

namespace wzq {

class ExecuteOnScopeExit : wzq::NonCopyAble {
   public:
    ExecuteOnScopeExit() = default;

    ExecuteOnScopeExit(ExecuteOnScopeExit&&) = default;
    ExecuteOnScopeExit& operator=(ExecuteOnScopeExit&&) = default;

    template <typename F, typename... Args>
    ExecuteOnScopeExit(F&& f, Args&&... args) {
        func_ = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    }

    ~ExecuteOnScopeExit() noexcept {
        if (func_) {
            func_();
        }
    }

   private:
    std::function<void()> func_;
};

}  // namespace wzq

#define _CONCAT(a, b) a##b
#define _MAKE_DEFER_(line) wzq::ExecuteOnScopeExit _CONCAT(defer, line) = [&]()

#undef WZQ_DEFER
#define WZQ_DEFER _MAKE_DEFER_(__LINE__)

#endif