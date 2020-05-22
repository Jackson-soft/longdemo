#pragma once

#include "noncopyable.hpp"
#include "sync_queue.hpp"
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

namespace Uranus::Utils
{
class ThreadPool : public Noncopyable
{
public:
    using Task = std::function<void()>;
    ThreadPool() : ThreadPool(0) {}

    ThreadPool(std::uint32_t num) : number(num == 0 ? std::thread::hardware_concurrency() : num)
    {
        threads.reserve(number);
        for (auto i = 0; i < number; ++i) {
            threads.emplace_back(std::make_shared<std::thread>(std::bind(&ThreadPool::dispatch, this)));
        }
    }

    ~ThreadPool()
    {
        if (running.load()) {
            running.store(false, std::memory_order_release);
            condition.notify_all();
            for (auto &it : threads) {
                // it.get()->join(); //线程自行消亡
                it->detach();
            }
            threads.clear();
        }
    }

    //提交任务到队列
    template<typename F, typename... Args>
    auto AddTask(F &&f, Args &&... args) -> std::future<std::result_of_t<F(Args...)>>
    {
        if (!running.load()) {
            throw std::runtime_error("task executor have closed commit.");
        }
        using resType = std::result_of_t<F(Args...)>;
        std::unique_lock<std::mutex> tLock(mutex);
        auto task = std::make_shared<std::packaged_task<resType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        tasks.Emplace([task]() { (*task)(); });
        condition.notify_one();
        std::future<resType> ret = task.get()->get_future();
        return ret;
    }

private:
    //任务分发
    void dispatch()
    {
        while (running.load()) {
            std::unique_lock<std::mutex> tLock(mutex);
            condition.wait(tLock, [this]() { return !tasks.Empty(); });
            Task task{tasks.Front()};
            tasks.Pop();  //删掉队列的头元素
            task();
        }
    }

    SyncQueue<Task> tasks;                              //任务队列
    std::vector<std::shared_ptr<std::thread>> threads;  //线程对象
    std::uint32_t number;                               //线程数
    std::mutex mutex;                                   //锁
    std::condition_variable condition;                  //条件变量
    std::atomic_bool running{true};                     //是否在运行
};
}  // namespace Uranus::Utils
