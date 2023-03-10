// #include <atomic>
#include <omp.h>
#include <chrono>
#include <iostream>
#include <mutex>
#include <opencv2/opencv.hpp>
using namespace std;

void fun(cv::Mat &hsv)
{
    auto start = std::chrono::steady_clock::now();
    auto hsv_ptr = hsv.ptr<cv::Vec3b>();
#pragma omp parallel for 
    for (int i = 0; i < hsv.rows * hsv.cols; ++i)
    {
        auto &vec = hsv_ptr[i];
        bool black = vec[1] <= 1.0 * 10 / 100 * 255 && vec[2] <= 0.3 * 255;
        // 绿色在80°~150°，且饱和度不能太低（否则变成灰色）
        bool green = (vec[0] >= 80.0 / 2) && (vec[0] <= 150.0 / 2) && (vec[1] >= 30.0 / 100 * 255);
        vec = cv::Vec3b{0, 0, 0};
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "single thread cost "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us"
              << std::endl;
}
int main()
{
    cv::Mat mat = cv::imread("A1.png");
    cv::Mat hsv1, hsv2;
    cv::cvtColor(mat, hsv1, cv::COLOR_BGR2HSV);
    cv::cvtColor(mat, hsv2, cv::COLOR_BGR2HSV);
    std::thread t1(fun, std::ref(hsv1));
    std::thread t2(fun, std::ref(hsv2));
    t1.join();
    t2.join();

    // 1. 单线程
    // int sum = 0;
    // auto hsv_ptr = hsv.ptr<cv::Vec3b>();
    // auto start = std::chrono::steady_clock::now();
    // for (int i = 0; i < hsv.rows * hsv.cols; ++i)
    // {
    //     auto &vec = hsv_ptr[i];
    //     bool black = vec[1] <= 1.0 * 10 / 100 * 255 && vec[2] <= 0.3 * 255;
    //     // 绿色在80°~150°，且饱和度不能太低（否则变成灰色）
    //     bool green = (vec[0] >= 80.0 / 2) && (vec[0] <= 150.0 / 2) && (vec[1] >= 30.0 / 100 *
    //     255); vec = cv::Vec3b{0, 0, 0};
    // }
    // auto end = std::chrono::steady_clock::now();
    // auto dur = (end - start);
    // std::cout << "sum: " << sum << std::endl;
    // std::cout << "single thread cost "
    //           << std::chrono::duration_cast<std::chrono::microseconds>(dur).count() << " us"
    //           << std::endl;

    //     // 2. 多线程 加锁
    //     std::mutex mtx;
    //     sum = 0;
    //     start = std::chrono::steady_clock::now();
    // #pragma omp parallel for num_threads(3)
    //     for (int i = 0; i < hsv.rows * hsv.cols; ++i)
    //     {
    //         auto &vec = hsv_ptr[i];
    //         bool black = vec[1] <= 1.0 * 10 / 100 * 255 && vec[2] <= 0.3 * 255;
    //         // 绿色在80°~150°，且饱和度不能太低（否则变成灰色）
    //         bool green = (vec[0] >= 80.0 / 2) && (vec[0] <= 150.0 / 2) && (vec[1] >= 30.0 / 100 *
    //         255); if (black || green)
    //         {
    //             std::lock_guard<std::mutex> lock(mtx);
    //             ++sum;
    //         }
    //     }
    //     end = std::chrono::steady_clock::now();
    //     dur = (end - start);
    //     std::cout << "sum: " << sum << std::endl;
    //     std::cout << "multi thread with mutex cost "
    //               << std::chrono::duration_cast<std::chrono::microseconds>(dur).count() << " us"
    //               << std::endl;

    //     // 3. 多线程 原子操作
    //     std::atomic<int> atomic_sum{0};
    //     start = std::chrono::steady_clock::now();
    // #pragma omp parallel for num_threads(3)
    //     for (int i = 0; i < hsv.rows * hsv.cols; ++i)
    //     {
    //         auto &vec = hsv_ptr[i];
    //         bool black = vec[1] <= 1.0 * 10 / 100 * 255 && vec[2] <= 0.3 * 255;
    //         // 绿色在80°~150°，且饱和度不能太低（否则变成灰色）
    //         bool green = (vec[0] >= 80.0 / 2) && (vec[0] <= 150.0 / 2) && (vec[1] >= 30.0 / 100 *
    //         255); if (black || green)
    //         {
    //             atomic_sum.fetch_add(1, std::memory_order_relaxed);
    //         }
    //     }
    //     end = std::chrono::steady_clock::now();
    //     dur = (end - start);
    //     std::cout << "sum: " << sum << std::endl;
    //     std::cout << "multi thread with atomic cost "
    //               << std::chrono::duration_cast<std::chrono::microseconds>(dur).count() << " us"
    //               << std::endl;

    //     atomic_sum = 0;

    //     return 0;
}
