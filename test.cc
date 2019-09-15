#include "common/defer.h"
#include "common/noncopyable.h"

#include <iostream>

// 默认是private继承，禁止运行时多态即 wzq::NonCopyAble x = new Test(); 会出现编译错误
class Test : wzq::NonCopyAble {
   public:
    Test() { std::cout << "Test constructor" << std::endl; }

    void Print() {
      std::cout << "Test " << std::endl;
    }

    ~Test() { std::cout << "Test deconstructor " << std::endl; }
};

int main() {
    Test a;
    WZQ_DEFER{
      a.Print();
    };
    std::cout << "2" << std::endl;

    return 0;
}