#include "src/ThreadPool.h"
#include <iostream>

int fun(int x, int y)
{
    return x + y;
}

int main()
{
    ThreadPool pool{3};

    std::future<int> ff = pool.Commit(fun, 3, 5);
    std::cout << ff.get() << std::endl;
    pool.Stop();
    return 0;
}
