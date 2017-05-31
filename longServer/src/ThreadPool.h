#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <vector>
#include <future>
#include <functional>
#include <type_traits>
#include <utility>


class ThreadPool
{
    using Task = std::function<void()>;
private:
    void ThreadWork(); //任务执行体

    std::queue<Task> tTasks; //任务队列
    std::vector<std::shared_ptr<std::thread>> tThreads; //线程对象
    unsigned int nThreadNum; //线程数

    std::mutex tMutex; //锁
    std::condition_variable tCondition; //条件变量
    std::atomic_bool bRunning; //是否在运行

public:
    ThreadPool(unsigned int num);
    ~ThreadPool();

    //提交任务到队列
    template<class F, class... Args>
    auto AddTask(F&& f, Args&&... args) ->std::future<typename std::result_of<F(Args...)>::type>;
};
