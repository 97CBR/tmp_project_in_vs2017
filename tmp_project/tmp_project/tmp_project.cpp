// tmp_project.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
// 下一步进行线程池优化

#include <iostream>
#include "CbrThreadPool.h"
#include <cstdlib>


void fun1() {
    const unsigned seed = time(nullptr); // Random generator seed
    srand(seed);
    Sleep(rand() % 500 + 100);
    cout << "current thread id : " << GetCurrentThreadId() << endl;
}

void fun2(const int id) {

    const unsigned seed = time(nullptr);  // Random generator seed
    srand(seed);
    Sleep(rand() % 500 + 10);
    if (id<3) {
      Sleep(10000);
    }
    cout << "current thread id : " << GetCurrentThreadId() << "\t i:" << id << endl;
}

int main() {
    // CbrThreadPool ctp;
    CbrThreadPool ctp(4, 16);
    // CbrThreadPool ctp(4, 16,2);

    // std::future<void> ff = ctp.PushTask(fun1);

    // for (int i = 0; i < 10; ++i)
    //     ctp.PushTask(fun1);


    for (int i = 0; i < 5000; ++i)
        ctp.PushTask(fun2, i);

    // ctp.Start();

    Sleep(60000);

    // ctp.Stop();

    std::cout << "Hello World!\n";
    return 0;
}


