#include <unistd.h>

#include <iostream>

void run()
{
	while (1) {
		/* code */
		std::cout << "child--" << std::endl;
		sleep(5);
	}
}
/*
 * 创建指定数目的子进程
 */
void createsubprocess(int num)
{
	pid_t pid;
	int i;
	for (i = 0; i < num; i++) {
		pid = fork();
		switch (pid) {
		case 0:
			run();
			break;
		default:
			/* code */
			break;
		}
	}
}

int main()
{
	int num = 3;
	createsubprocess(num);
	while (1) {
		/* code */
		std::cout << "dady=====" << std::endl;
		sleep(6);
	}
	return 0;
}
