#include <stdio.h>
#include <unistd.h>

int main()
{
	char* arg[2];
	pid_t pid = getpid();

	arg[0] = "./after";
	arg[1] = 0;

	printf("Before execvp() : %d\n", pid);

	execvp(arg[0], arg);

	return 0;
}
