#pragma once

#include "SyncQueue.hpp"
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

class ThreadPool
{
    using Task = std::function<void()>;

private:
    void ThreadWork(); //任务执行体

    SyncQueue<Task> tTasks;								//任务队列
    std::vector<std::shared_ptr<std::thread>> tThreads; //线程对象
    unsigned short nThreadNum;							//线程数

    std::mutex tMutex;					//锁
    std::condition_variable tCondition; //条件变量
    std::atomic_bool bRunning = true;   //是否在运行

public:
    ThreadPool(unsigned short num);
    ~ThreadPool();

    //提交任务到队列
    template <typename F, typename... Args>
    auto AddTask(F &&f, Args &&... args)
        -> std::future<std::result_of_t<F(Args...)>>
    {
        if (!bRunning.load()) {
            throw std::runtime_error("task executor have closed commit.");
        }
        using resType = std::result_of_t<F(Args...)>;
        std::unique_lock<std::mutex> tLock(tMutex);
        auto task = std::make_shared<std::packaged_task<resType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        tTasks.Emplace([task]() { (*task)(); });
        tCondition.notify_one();
        std::future<resType> ret = task.get()->get_future();
        return ret;
    }
};
