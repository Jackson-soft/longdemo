#include <iostream>


template<int N, int M>
struct meta_fun
{
    static const int value = N + M;
};

int main()
{
    std::cout << meta_fun<3, 5>::value << std::endl;
    return 0;
}
