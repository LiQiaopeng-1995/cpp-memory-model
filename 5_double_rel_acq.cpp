#include <assert.h>

#include <atomic>
#include <iostream>
#include <thread>

using namespace std;

atomic<bool> x, y;
atomic<int> z;

void write_x(){
    x.store(true, memory_order_release);
}

void write_y(){
    y.store(true, memory_order_release);
}

void read_x_then_y(){
    while(!x.load(memory_order_acquire));
    if (y.load(memory_order_acquire)){
        cout << "read x then y" << endl;
        z++;
    }
}

void read_y_then_x(){
    while(!y.load(memory_order_acquire));
    if (x.load(memory_order_acquire)){
        cout << "read y then x" << endl;
        z++;
    }
}

int main(){
    x = false;
    y =false;
    z = 0;
    thread t1(write_x);
    thread t2(write_y);
    thread t3(read_x_then_y);
    thread t4(read_y_then_x);
    t3.join();
    t4.join();
    t1.join();
    t2.join();
    cout << "z: " << z << endl;
    assert(z.load()!=0);
    return 0;
}