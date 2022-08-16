#include <iostream>
#include <thread>
#include <atomic>
using namespace std;

std::atomic<int> x = 0;
std::atomic<int> y = 0;
int r0 = 100;
int r1 = 100;

void f1() {
    x.store(1, memory_order_relaxed);
    asm volatile("": : :"memory"); // 创建一个编译器级内存屏障，强迫优化器不重新排序内存访问权限
    r0 = y;
}

void f2() {
    y.store(1, memory_order_relaxed);
    asm volatile("": : :"memory");  // 创建一个编译器级内存屏障，强迫优化器不重新排序内存访问权限
    r1 = x;
}

void init() {
    x = 0;
    y = 0;
    r0 = 100;
    r1 = 100;
}

bool check() {
    return r0 == 0 && r1 == 0;
}

// 原子变量
std::atomic<bool> wait1{true}; 
std::atomic<bool> wait2{true};  
std::atomic<bool> stop{false};

void loop1() {
    while(!stop.load(std::memory_order_relaxed)) {
        // 等到wait1被赋值false, 才进行操作
        while (wait1.load(std::memory_order_relaxed)){}; 

        asm volatile("" ::: "memory");  // 阻止编译器优化
        f1();
        asm volatile("" ::: "memory");

        wait1.store(true, std::memory_order_relaxed);
    }
}

void loop2() {
    while (!stop.load(std::memory_order_relaxed)) {
        while (wait2.load(std::memory_order_relaxed));

        asm volatile("" ::: "memory");
        f2();
        asm volatile("" ::: "memory");

        wait2.store(true, std::memory_order_relaxed);
    }
}

int main() {
    std::thread thread1(loop1);
    std::thread thread2(loop2);

    long count = 0;
    while(true) {
        count++;
        init();

        asm volatile("" ::: "memory");
        // 线程执行f函数
        wait1.store(false, std::memory_order_relaxed);
        wait2.store(false, std::memory_order_relaxed);

        // 线程执行完
        while (!wait1.load(std::memory_order_relaxed));
        while (!wait2.load(std::memory_order_relaxed));
        asm volatile("" ::: "memory");
        
        if (check()) {
            std::cout << "test count " << count << ": r0 == " << r0 << " && r1 == " << r1 << std::endl;
            break;
        } else {
            if (count % 10000 == 0) {
                std::cout << "test count " << count << ": OK" << std::endl;
            }
        }
    }

    stop.store(true);
    wait1.store(false);
    wait2.store(false);
    thread1.join();
    thread2.join();
    return 0;
}