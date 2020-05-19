// tmp_project.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
// 下一步进行线程池优化

#include <iostream>
#include "CbrThreadPool.h"
#include <cstdlib>


void fun1() {
    const unsigned seed = time(nullptr); // Random generator seed
    srand(seed);
    Sleep(rand() % 500 + 100);
    std::cout << "current thread id : " << GetCurrentThreadId() << std::endl;
}

void fun2(const int id) {

    const unsigned seed = time(nullptr);  // Random generator seed
    srand(seed);
    Sleep(rand() % 500 + 10);
    if (id < 3)
        Sleep(10000);
    std::cout << "current thread id : " << GetCurrentThreadId() << "\t i:" << id << std::endl;
}

int fun3(const int id) {
    const unsigned seed = time(nullptr);  // Random generator seed
    srand(seed);
    const auto gg = id + rand() % 500 + 10;
    return gg;
}


int main() {
    // CbrThreadPool ctp;
    CbrThreadPool ctp(4, 16);
    // CbrThreadPool ctp(4, 16,2);

    // std::future<void> ff = ctp.PushTask(fun1);

    // for (int i = 0; i < 10; ++i)
    //     ctp.PushTask(fun1);

    std::vector<std::future<int>> res;

    res.reserve(5000);
    for (int i = 0; i < 500; ++i) {
        // auto hh = ;
        res.push_back(ctp.PushTask(fun3, i));
    }
    for (auto& element : res)
        std::cout << "测试结果" << element.get() << std::endl;
    // ctp.Start();

    Sleep(60000);

    // ctp.Stop();

    std::cout << "Hello World!\n";
    return 0;
}


