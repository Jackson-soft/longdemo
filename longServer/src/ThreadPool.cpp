#include "ThreadPool.h"

ThreadPool::ThreadPool(unsigned short num) : nThreadNum(num)
{
	if (nThreadNum == 0) {
        nThreadNum = std::thread::hardware_concurrency();
	}
    tThreads.reserve(nThreadNum);
	for (unsigned short i = 0; i < nThreadNum; ++i) {
		tThreads.emplace_back(std::make_shared<std::thread>(
			std::bind(&ThreadPool::ThreadWork, this)));
	}
}

ThreadPool::~ThreadPool()
{
	if (bRunning.load()) {
		bRunning.store(false, std::memory_order_release);
		tCondition.notify_all();
		for (auto &it : tThreads) {
			// it.get()->join(); //线程自行消亡
			it.get()->detach();
		}
        tThreads.clear();
	}
}

void ThreadPool::ThreadWork()
{
	while (bRunning.load()) {
		std::unique_lock<std::mutex> tLock(tMutex);
        tCondition.wait(tLock, [this]() { return !tTasks.Empty(); });
        Task task{std::move(tTasks.Front())};
        tTasks.Pop(); //删掉队列的头元素
		task();
	}
}
