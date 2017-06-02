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
    template<typename F, typename... Args>
    auto AddTask(F&& f, Args&&... args) ->std::future<typename std::result_of<F(Args...)>::type>;
};


template<typename F, typename... Args>
auto ThreadPool::AddTask(F&& f, Args&&... args) ->std::future<typename std::result_of<F(Args...)>::type>
{
    if(!bRunning.load()){
        throw std::runtime_error("task executor have closed commit.");
    }
    using resType = typename std::result_of<F(Args...)>::type;
    std::unique_lock<std::mutex> tLock(tMutex);
    auto task = std::make_shared<std::packaged_task<resType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    tTasks.emplace([task](){(*task)();});
    tCondition.notify_one();
    std::future<resType> ret = task.get()->get_future();
    return ret;
}
