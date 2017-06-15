#include <experimental/filesystem>
#include <iostream>
#include <string>

template <int N, int M>
struct meta_fun {
	static const int value = N + M;
};

int main()
{
	std::cout << "Current root name is: "
			  << std::experimental::filesystem::current_path().root_name()
			  << '\n';
	std::experimental::filesystem::create_directory(
		std::experimental::filesystem::path("log"));
	std::string d = "llll";
	std::experimental::filesystem::create_directory(d);
	std::cout << meta_fun<3, 5>::value << std::endl;
	return 0;
}
