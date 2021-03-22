#include <iostream>
#include <memory>
#include <mutex>

#include "MemoryDetect.h"

struct A {
    int a;
};

int main() {
    printf("memory detect \n");
    int *p1 = new int;
    delete p1;

    int *p2 = new int[4];
    delete[] p2;

    A *a1 = new A;
    // delete a1;

    A *a2 = new A[1];
    delete[] a2;

    { std::shared_ptr<A> a = std::make_shared<A>(); }
    checkLeaks();
    return 0;
}
