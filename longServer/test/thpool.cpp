#include <iostream>
#include "../ThreadPool.h"

int main()
{
    ThreadPool pool(4);
    auto result = pool.AddTask([](int answer) { return answer; }, 42);

    // get result from future
    std::cout << result.get() << std::endl;
    return 0;
}
