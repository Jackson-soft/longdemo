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

namespace uranus::utils {
class ThreadPool : public Noncopyable {
public:
    using Task = std::function<void()>;

    ThreadPool() : ThreadPool(0) {}

    explicit ThreadPool(std::uint32_t num) : threadNum_(num) {
        if (threadNum_ == 0U) {
            // 线程池大小配置为CPU核数
            threadNum_ = std::thread::hardware_concurrency();
        }
        threads_.reserve(threadNum_);
        for (std::uint32_t i = 0; i < threadNum_; ++i) {
            threads_.emplace_back(std::make_shared<std::thread>([this] {
                dispatch();
            }));
        }
    }

    ~ThreadPool() {
        if (running_.load()) {
            running_.store(false, std::memory_order_release);
            condition_.notify_all();
            for (auto &it : threads_) {
                // it.get()->join(); //线程自行消亡
                it->detach();
            }
            threads_.clear();
        }
    }

    // 提交任务到队列
    template<typename F, typename... Args>
    auto AddTask(F &&f, Args &&...args) -> std::future<std::result_of_t<F(Args...)>> {
        if (!running_.load()) {
            throw std::runtime_error("task executor have closed commit.");
        }
        using resType = std::result_of_t<F(Args...)>;
        std::unique_lock<std::mutex> tLock(mutex_);
        auto                         task = std::make_shared<std::packaged_task<resType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        tasks_.Emplace([task]() {
            (*task)();
        });
        condition_.notify_one();
        std::future<resType> ret = task.get()->get_future();
        return ret;
    }

private:
    // 任务分发
    void dispatch() {
        while (running_.load()) {
            std::unique_lock<std::mutex> tLock(mutex_);
            condition_.wait(tLock, [this]() {
                return !tasks_.Empty();
            });
            Task task{std::move(tasks_.Front())};
            tasks_.Pop();  // 删掉队列的头元素
            task();
        }
    }

    SyncQueue<Task>                           tasks_{};        // 任务队列
    std::vector<std::shared_ptr<std::thread>> threads_;        // 线程对象
    std::uint32_t                             threadNum_;      // 线程数
    std::mutex                                mutex_;          // 锁
    std::condition_variable                   condition_;      // 条件变量
    std::atomic_bool                          running_{true};  // 是否在运行
};
}  // namespace uranus::utils