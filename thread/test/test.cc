#include <iostream>
#include <thread>

#include "thread/count_down_latch.h"
#include "thread/thread_pool.h"

void TestThreadPool() {
    cout << "hello" << endl;
    wzq::ThreadPool pool(wzq::ThreadPool::ThreadPoolConfig{4, 5, 6, std::chrono::seconds(4)});
    pool.Start();
    std::this_thread::sleep_for(std::chrono::seconds(4));
    cout << "thread size " << pool.GetTotalThreadSize() << endl;
    std::atomic<int> index;
    index.store(0);
    std::thread t([&]() {
        for (int i = 0; i < 10; ++i) {
            pool.Run([&]() {
                cout << "function " << index.load() << endl;
                std::this_thread::sleep_for(std::chrono::seconds(4));
                index++;
            });
            // std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    });
    t.detach();
    cout << "=================" << endl;

    std::this_thread::sleep_for(std::chrono::seconds(4));
    pool.Reset(wzq::ThreadPool::ThreadPoolConfig{4, 4, 6, std::chrono::seconds(4)});
    std::this_thread::sleep_for(std::chrono::seconds(4));
    cout << "thread size " << pool.GetTotalThreadSize() << endl;
    cout << "waiting size " << pool.GetWaitingThreadSize() << endl;
    cout << "---------------" << endl;
    // pool.ShutDownNow();
    getchar();
    cout << "world" << endl;
}

int main() {
    TestThreadPool();
    return 0;
    std::cout << "hello world " << std::endl;
    wzq::CountDownLatch t(5);
    std::thread thread([&] {
        t.Await(0);
        std::cout << "await over" << std::endl;
    });
    for (int i = 0; i < 5; i++) {
        t.CountDown();
        std::cout << "count " << t.GetCount() << std::endl;
    }
    getchar();
    return 0;
}