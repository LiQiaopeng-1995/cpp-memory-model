#include <assert.h>

#include <atomic>
#include <iostream>
#include <thread>
using namespace std;
atomic<int> z;
atomic<bool> x, y;

void write_x_then_y()
{
    // atomic_thread_fence(memory_order_release);
    x.store(true, memory_order_relaxed);
    y.store(true, memory_order_relaxed);
}

void read_y_then_x()
{
    while (!y.load(memory_order_relaxed));
    // atomic_thread_fence(memory_order_acquire);
    if (x.load(memory_order_relaxed)) ++z;
}

void test()
{
    x = false;
    y = false;
    z = 0;
    thread t1(write_x_then_y);
    thread t2(read_y_then_x);
    t1.join();
    t2.join();
    assert(z.load() != 0);
}
int main()
{
    for (long int i = 0; i < 1e6+1; i++)
    {
        test();
        if (i % 10000 == 0) cout << i << " time OK" << endl;
    }
    return 0;
}