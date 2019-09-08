#include <iostream>
#include "thread/count_down_latch.h"
#include <thread>

int main() {
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