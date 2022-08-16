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
    while (!y.load(memory_order_acquire))  // 3, 等到2的执行结束(自旋)
        ;
    if (x.load(memory_order_relaxed))  // 4
    {
        ++z;
    }
}

int main()
{
    x = false;
    y = false;
    z = 0;
    cout << "t1 t2" << endl;
    thread t1(write_x_then_y);
    thread t2(read_y_then_x);
    t1.join();
    t2.join();
    assert(z.load() != 0);
    cout << "assert success" << endl;
    return 0;
}