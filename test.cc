#include "common/noncopyable.h"

#include <iostream>

// 默认是private继承，禁止运行时多态即 wzq::NonCopyAble x = new Test(); 会出现编译错误
class Test : wzq::NonCopyAble {
   public:
    Test() { std::cout << "Test constructor" << std::endl; }

    ~Test() { std::cout << "Test deconstructor " << std::endl; }
};

int main() {
    Test a;
    return 0;
}