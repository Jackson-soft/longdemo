#include "../../longServer/include/TimeUtil.hpp"

#include <iostream>
#include <string>

int main() {
    std::string a = TimeUtil::GetCurrentDay();
    std::cout << a << std::endl;
    std::string b = TimeUtil::GetCurrentTime();
    std::cout << b << std::endl;
    return 0;
}
