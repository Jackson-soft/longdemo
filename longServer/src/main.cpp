#include <future>
#include <iostream>
#include <string>
#include <vector>

#include "Formatter.hpp"
#include "LogLevel.hpp"
#include "ThreadPool.hpp"

int fun(int x, int y) { return x + y; }

int main()
{
    std::vector<int> nTest{3, 4, 5};
    for (auto &i : nTest) {
        std::cout << i << std::endl;
    }
    /*
        ThreadPool pool;
        auto f = pool.AddTask(fun, 3, 5);
        //
        std::cout << f.get() << std::endl;

        f = pool.AddTask([](int answer) { return answer; }, 42);

        std::cout << f.get() << std::endl;

         f = pool.AddTask([]() { std::cout << "this is text!" << std::endl; });
         f = pool.AddTask(fun1);

         std::cout << f.get() << std::endl;

        std::cout << TimeUtil::GetCurrentDay() << std::endl;
        std::cout << TimeUtil::GetCurrentTime() << std::endl;
        auto f1 = std::async(std::launch::async, []() { return 9; });
        std::cout << f1.get() << std::endl;
        */
    return 0;
}
