#include <assert.h>

#include <atomic>
#include <iostream>
#include <thread>
using namespace std;
atomic<int> sync;

void thread_1() { sync.store(1, memory_order_release); }

void thread_2()
{
    int expected = 1;
    while (!sync.compare_exchange_weak(expected, 2, memory_order_acq_rel)) expected = 1;
}

void thread_3()
{
    int a = -1;
    while (sync.load(memory_order_acquire) < 2)
        ;
    a = sync.load(memory_order_acquire);
    cout << a << endl;
}

int main()
{
    sync = 0;
    thread t1(thread_1);
    thread t2(thread_2);
    thread t3(thread_3);
    t1.join();
    t2.join();
    t3.join();
}
