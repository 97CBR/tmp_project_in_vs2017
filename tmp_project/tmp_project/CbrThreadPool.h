#pragma once

#include <atomic>
#include <future>
#include <iostream>
#include <vector>
#include <queue>
#include <mutex>
#include <Windows.h>


using namespace std;

class CbrThreadPool {
    public:
        CbrThreadPool();
        CbrThreadPool(int min, int max);
        ~CbrThreadPool();
        template <class F, class ... Args>
        auto PushTask(F&& f, Args&& ... args)->std::future<decltype(f(args...))>;
        //
        // template <class F>
        // auto PushTask(F&& f)->std::future<decltype(f(0))>;

        void Stop();
        void Start();
    private:
        void Init(int min, int max);
        void CreateThread();


    private:
        using task = std::function<void()>;

        mutex task_lock_;
        condition_variable condition_;
        atomic<int> current_idle_thread_;
        atomic<bool> stop_;
        vector<thread> threads_pool_;
        queue<task> tasks_;
};

template<class F, class... Args>
auto CbrThreadPool::PushTask(F&& f, Args&& ... args) -> std::future<decltype(f(args...))> {

    using RetType = decltype(f(args...));
    auto pack_task = std::make_shared<packaged_task<RetType()>>(
                         std::bind(std::forward<F>(f), std::forward<Args>(args)...)
                     );

    std::lock_guard<std::mutex> lock{ task_lock_ };
	tasks_.push(
		[pack_task]() {
		    (*pack_task)();
		}
    );

    this->condition_.notify_one();
    return pack_task->get_future();

}
