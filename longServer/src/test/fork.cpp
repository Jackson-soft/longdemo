#include <array>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */
#include <unistd.h>

int main(int argc, char *argv[])
{
	struct Workor {
		pid_t pid;
	};
	std::array<Workor, 2> mName;
	pid_t pid;
	for (int i = 0; i < 4; ++i) {
		pid = fork();
		if (pid == 0 || pid == -1) {
			break;
		} else {
			std::cout << "ccc: " << i << std::endl;
		}
	}

	std::cout << "dadfsdf: " << pid << std::endl;
	return 0;
}
