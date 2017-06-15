#include <iostream>
#include <string>

#include "src/ThreadPool.h"

int fun(int x, int y) { return x + y; }

int main()
{
    ThreadPool pool(5);
    auto f = pool.AddTask(fun, 3, 5);
    // auto f = pool.AddTask([](int answer) { return answer; }, 42);
    std::cout << f.get() << std::endl;
    return 0;
}
