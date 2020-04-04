#include <iostream>
#include <thread>

#include "common/cmd.h"
#include "common/defer.h"
#include "common/noncopyable.h"
#include "common/own_strings.h"
#include "timer/timer.h"

// 默认是private继承，禁止运行时多态即 wzq::NonCopyAble x = new Test(); 会出现编译错误
class Test : wzq::NonCopyAble {
   public:
    Test() { std::cout << "Test constructor" << std::endl; }

    void Print() { std::cout << "Test " << std::endl; }

    ~Test() { std::cout << "Test deconstructor " << std::endl; }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    if (v.empty()) {
        return os;
    }
    os << v.front();

    for (std::size_t i = 1; i < v.size(); ++i) {
        os << ' ' << v[i];
    }
    return os;
}

void TestTimerQueue() {
    wzq::TimerQueue q;
    q.Run();
    for (int i = 5; i < 15; ++i) {
        q.AddFuncAfterDuration(std::chrono::seconds(i + 1), [i]() { std::cout << "this is " << i << std::endl; });

        q.AddFuncAtTimePoint(std::chrono::high_resolution_clock::now() + std::chrono::seconds(1),
                             [i]() { std::cout << "this is " << i << " at " << std::endl; });
    }

    int id = q.AddRepeatedFunc(10, std::chrono::seconds(1), []() { std::cout << "func " << std::endl; });
    std::this_thread::sleep_for(std::chrono::seconds(4));
    q.CancelRepeatedFuncId(id);

    std::this_thread::sleep_for(std::chrono::seconds(30));
    q.Stop();
}

int main() {
    TestTimerQueue();
    return 0;
    Test a;
    WZQ_DEFER { a.Print(); };
    std::cout << "2" << std::endl;

    std::cout << wzq::Command::RunCmd("ls");

    std::vector<std::string> words = {"What", "a", "beautiful", "world"};
    std::cout << words << '\n';

    return 0;
}