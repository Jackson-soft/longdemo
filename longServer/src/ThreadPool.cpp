#include "ThreadPool.h"


ThreadPool::ThreadPool(unsigned int num) : nThreadNum(num), bRunning(true)
{
    if(nThreadNum == 0){
        nThreadNum = std::thread::hardware_concurrency();
    }
    for(unsigned int i = 0; i < nThreadNum; ++i){
        tThreads.emplace_back(std::make_shared<std::thread>(std::bind(&ThreadPool::ThreadWork, this)));
    }
}


ThreadPool::~ThreadPool()
{
    if(bRunning.load()){
        bRunning.store(false, std::memory_order_release);
        tCondition.notify_all();
        for(auto &it : tThreads){
            //it.get()->join(); //线程自行消亡
            it.get()->detach();
        }
        bRunning = false;
    }
}

template<class F, class... Args>
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
    std::future<resType> ret = task.get_future();
    return ret;
}


void ThreadPool::ThreadWork()
{
    while(bRunning.load()){
        std::unique_lock<std::mutex> tLock(tMutex);
        tCondition.wait(tLock, [this](){return !tTasks.empty();});
        Task task{std::move(tTasks.front())};
        tTasks.pop(); //删掉队列的头元素
        task();
    }
}

