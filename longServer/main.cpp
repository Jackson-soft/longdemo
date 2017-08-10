#include <future>
#include <iostream>
#include <string>

#include "src/ThreadPool.h"
#include "src/TimeUtil.hpp"

int fun(int x, int y) { return x + y; }

int main()
{
    /*
    ThreadPool pool(5);
    auto f = pool.AddTask(fun, 3, 5);
    // auto f = pool.AddTask([](int answer) { return answer; }, 42);
    std::cout << f.get() << std::endl;
    */
    std::cout << TimeUtil::GetCurrentDay() << std::endl;
    std::cout << TimeUtil::GetCurrentTime() << std::endl;
    auto f1 = std::async(std::launch::async, []() { return 9; });
    std::cout << f1.get() << std::endl;
    return 0;
}
