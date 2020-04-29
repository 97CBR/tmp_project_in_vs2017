#include "CbrThreadPool.h"

#include <future>

CbrThreadPool::CbrThreadPool(): min_(1), max_(4) {

    cout << "创建默认线程池" << endl;
    current_waiting_tasks_number_.store(0);
    current_threads_number_.store(0);
    current_idle_thread_.store(0);
    current_tasks_number_.store(0);
    terminal_.store(false);

    // stop_.store(true);

    Init(1, 4);
}

CbrThreadPool::CbrThreadPool(const int min = 2, const int max = 4)
    : min_(min), max_(max) {

    cout << "创建指定线程池" << endl;
    Init(min, max);
}

CbrThreadPool::~CbrThreadPool() {
    Stop();
    cout << "销毁" << endl;
}

// 终止所有线程，清除所有队列
void CbrThreadPool::Stop() {

    cout << "终止线程" << endl;
    stop_.store(true);

    condition_.notify_all();
    for (auto& element : threads_pool_) {
        if (element.joinable())
            element.join();
    }

    while (!this->tasks_.empty()) {
        // 清除队列
        const auto clear_task = &tasks_.front();
        this->tasks_.pop();
        delete clear_task;
    }

}

// 终止所有线程，清除所有队列
void CbrThreadPool::Start() {

    cout << "开始线程" << endl;
    stop_.store(false);
    // condition_.notify_all();

}


// 初始化线程
void CbrThreadPool::Init(const int min = 2, const int max = 4) {
    // current_threads_number_.store(min);
    stop_.store(false);
    for (auto i = 0; i < min; ++i) {
        std::unique_lock<std::mutex> lock{this->thread_lock_};
        threads_pool_.emplace_back([this]() {
            this->CreateThread();
        });

    }
    thread watch(&CbrThreadPool::DynamicAdjustThreadNumber, this, min, max);
    watch.detach();
}

// 动态增减线程数
void CbrThreadPool::DynamicAdjustThreadNumber(const int min = 2, const int max = 4) {
    while (true) {
        if (this->stop_)
            return;
        if (this->current_threads_number_.load() < max && (this->current_tasks_number_.load() > min)) {
            std::unique_lock<std::mutex> lock{this->thread_lock_};
            threads_pool_.emplace_back([this]() {
                this->CreateThread();
                // lock.unlock();
                // lock.lock();
            });
        }
        if (this->current_tasks_number_.load() < current_threads_number_.load()
                && this->current_threads_number_.load() > min) {
            std::lock_guard<std::mutex> lock{task_lock_};
            this->terminal_.store(true);
            condition_.notify_one();
            // this->terminal_.store(false);
        }
        if (current_idle_thread_.load() > 0) {
            std::lock_guard<std::mutex> lock{ task_lock_ };
            this->condition_.notify_one();

        }
    }

}


// 抽象出来的线程创建函数
void CbrThreadPool::CreateThread() {
    ++current_threads_number_;
    ++current_idle_thread_;
    cout << "创建线程" << endl;

    while (true) {
        function<void()> execute_task;

        // 等待锁、以及判断是否有终止标志、以及任务队列是否为空
        ++current_waiting_tasks_number_;
        unique_lock<mutex> lock{this->task_lock_};



        //如果任务队列为空，则进去等待
        if (tasks_.empty()) {
            condition_.wait(lock, [this]() {
                return this->stop_.load() || !this->tasks_.empty();
            });
            cout << "收到通知" << endl;

            if (this->current_threads_number_.load() > min_) {
                --this->current_idle_thread_;
                --this->current_threads_number_;
                cout << "销毁多余线程" << endl;
                this->terminal_.store(false);
                // this->threads_pool_.
                return;
            }

            // 配合监控线程数的线程进行线程数控制

            // if (this->terminal_.load()) {
            //     --this->current_idle_thread_;
            //     --this->current_threads_number_;
            //     cout << "销毁多余线程" << endl;
            //     this->terminal_.store(false);
            //     // this->threads_pool_.
            //     return;
            // }

        } else {
            --current_waiting_tasks_number_;
            // 停止标志，以及任务队列空则销毁线程
            // TODO: 考虑添加一个最少线程判断在下方
            if (this->stop_.load() && this->tasks_.empty())
                return;

            execute_task = move(this->tasks_.front());
            this->tasks_.pop();
            --this->current_idle_thread_;

            cout << "current idle thread number：" << current_idle_thread_.load() << "\tcurrent thread number：" <<
                 current_threads_number_.load() << endl;
            cout << "current tasks number：" << current_tasks_number_.load() << "\tcurrent waiting tasks number：" <<
                 current_waiting_tasks_number_.load() << endl;

            lock.unlock();
            execute_task(); //running task
            lock.lock();
            --current_tasks_number_;

            ++this->current_idle_thread_;
        }

        if (this->stop_.load())
            break;
    }
}


// 抽象出来的线程创建函数
void CbrThreadPool::CreateThread(const int timeout = 5) {
    ++current_threads_number_;
    ++current_idle_thread_;
    cout << "创建线程" << endl;
    while (true) {
        function<void()> execute_task;
        unique_lock<mutex> lock{ this->task_lock_ };
        // 等待锁、以及判断是否有终止标志、以及任务队列是否为空
        ++current_waiting_tasks_number_;
        condition_.wait_for(lock, chrono::seconds(timeout), [this]() {
            return this->stop_.load() || !this->tasks_.empty();
        });
        cout << "收到通知" << endl;
        --current_waiting_tasks_number_;
        // 停止标志，以及任务队列空则销毁线程
        // TODO: 考虑添加一个最少线程判断在下方
        if (this->stop_.load() && this->tasks_.empty())
            return;

        // 配合监控线程数的线程进行线程数控制
        if (this->terminal_.load()) {
            --this->current_idle_thread_;
            --this->current_threads_number_;
            cout << "销毁多余线程" << endl;
            this->terminal_.store(false);
            // this->threads_pool_.
            return;
        }
        execute_task = move(this->tasks_.front());
        this->tasks_.pop();
        --this->current_idle_thread_;

        cout << "current idle thread number：" << current_idle_thread_.load() << "\tcurrent thread number：" << current_threads_number_.load() << endl;
        cout << "current tasks number：" << current_tasks_number_.load() << "\tcurrent waiting tasks number：" << current_waiting_tasks_number_.load() << endl;

        execute_task();//running task
        --current_tasks_number_;

        ++this->current_idle_thread_;
    }
}
