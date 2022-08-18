#include <assert.h>

#include <atomic>
#include <iostream>
#include <thread>

using namespace std;

atomic<bool> x, y;
atomic<int> z;

void write_x_then_y()
{
    x.store(true, memory_order_relaxed);  // 1
    y.store(true, memory_order_release);  // 2
}

void read_y_then_x()
{
    while (!y.load(memory_order_acquire));  // 3, 等到2的执行结束(自旋)
    if (x.load(memory_order_relaxed))  // 4
    {
        ++z;
    }
    // y.load(memory_order_acquire);
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
    assert(z.load() == 1);
}
int main()
{
    for (int i = 0; i < 1e8; ++i) {
        test();
        if (i%1000 == 0)
            std::cout << i << " times: OK" << std::endl;
    }
    return 0;
}