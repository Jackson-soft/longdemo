#include <iostream>
#include <string>

#include "src/ThreadPool.h"

auto fun(int x, int y)
{
    return x + y;
}

int main()
{
    ThreadPool pool{5};
    auto f = pool.Commit(fun, 3, 5);
    std::cout << f.get() << std::endl;
    pool.Stop();
    return 0;
}
