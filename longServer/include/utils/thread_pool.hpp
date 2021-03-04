#pragma once

#include "utils/noncopyable.hpp"
#include "utils/sync_queue.hpp"
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace uranus::utils
{
class ThreadPool: public Noncopyable
{
public:
    using Task = std::function<void()>;

    ThreadPool(): ThreadPool(0) {}

    ThreadPool(std::uint32_t num): mThreadNum(num)
    {
        if (!mThreadNum) {
            // 线程池大小配置为CPU核数
            mThreadNum = std::thread::hardware_concurrency();
        }
        mThreads.reserve(mThreadNum);
        for (std::uint32_t i = 0; i < mThreadNum; ++i) {
            mThreads.emplace_back(std::make_shared<std::thread>(std::bind(&ThreadPool::dispatch, this)));
        }
    }

    ~ThreadPool()
    {
        if (mRunning.load()) {
            mRunning.store(false, std::memory_order_release);
            mCondition.notify_all();
            for (auto &it : mThreads) {
                // it.get()->join(); //线程自行消亡
                it->detach();
            }
            mThreads.clear();
        }
    }

    //提交任务到队列
    template<typename F, typename... Args>
    auto AddTask(F &&f, Args &&...args) -> std::future<std::result_of_t<F(Args...)>>
    {
        if (!mRunning.load()) {
            throw std::runtime_error("task executor have closed commit.");
        }
        using resType = std::result_of_t<F(Args...)>;
        std::unique_lock<std::mutex> tLock(mMutex);
        auto task = std::make_shared<std::packaged_task<resType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        mTasks.Emplace([task]() { (*task)(); });
        mCondition.notify_one();
        std::future<resType> ret = task.get()->get_future();
        return ret;
    }

private:
    //任务分发
    void dispatch()
    {
        while (mRunning.load()) {
            std::unique_lock<std::mutex> tLock(mMutex);
            mCondition.wait(tLock, [this]() { return !mTasks.Empty(); });
            Task task{std::move(mTasks.Front())};
            mTasks.Pop();  //删掉队列的头元素
            task();
        }
    }

    SyncQueue<Task> mTasks{};                            //任务队列
    std::vector<std::shared_ptr<std::thread>> mThreads;  //线程对象
    std::uint32_t mThreadNum;                            //线程数
    std::mutex mMutex;                                   //锁
    std::condition_variable mCondition;                  //条件变量
    std::atomic_bool mRunning{true};                     //是否在运行
};
}  // namespace uranus::utils