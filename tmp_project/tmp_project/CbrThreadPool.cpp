#include "CbrThreadPool.h"

#include <future>

CbrThreadPool::CbrThreadPool() {

    cout << "创建默认线程池" << endl;
    Init(2, 4);
}

CbrThreadPool::CbrThreadPool(const int min = 2, const int max = 4) {

    cout << "创建指定线程池" << endl;
    Init(min, max);
}

CbrThreadPool::~CbrThreadPool() {
    Stop();
    cout << "销毁" << endl;
}

// 终止所有进程，清除所有队列
void CbrThreadPool::Stop() {

    cout << "终止线程" << endl;
    stop_.store(true);
    while (!this->tasks_.empty()) {
        // 清除队列
        const auto clear_task = move(&tasks_.front());
        this->tasks_.pop();
        delete clear_task;
    }
    condition_.notify_all();
    for (auto& element : threads_pool_) {
        if (element.joinable())
            element.join();
    }
}

// 终止所有进程，清除所有队列
void CbrThreadPool::Start() {

    cout << "开始线程" << endl;
    stop_.store(false);
    condition_.notify_all();

}


// 初始化线程
void CbrThreadPool::Init(const int min = 2, const int max = 4) {
    current_idle_thread_.store(min);
    stop_.store(false);
    for (auto i = 0; i < min; ++i) {
        threads_pool_.emplace_back([this]() {
            CreateThread();
        });
    }
}



// 抽象出来的线程创建函数
void CbrThreadPool::CreateThread() {
    while (true) {
        function<void()> execute_task;
        unique_lock<mutex> lock { this->task_lock_ };
        // 等待锁、以及判断是否有终止标志、以及任务队列是否为空
        condition_.wait(lock, [this]() {
            return this->stop_.load() || !this->tasks_.empty();
        });
        // 停止标志，以及任务队列空则销毁进程
        // TODO: 考虑添加一个最少线程判断在下方
        if (this->stop_ && this->tasks_.empty())
            return;
        execute_task = move(this->tasks_.front());
        this->tasks_.pop();
        --this->current_idle_thread_;
        execute_task();//running task
        ++this->current_idle_thread_;
    }
}


