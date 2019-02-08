#include <sys/ptrace.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Program name not found!\n");
		return -1;
	}
	pid_t pid = fork();
	if(pid == 0)
	{
		//child
		if(ptrace(PTRACE_TRACEME, 0, 0, 0) < 0)
		{
			printf("Error in ptrace\n");
			return -1;
		}
		printf("Now execl\n");
		execl(argv[1], argv[1], NULL);
	}
	else if(pid >= 1)
	{
		//parent
		printf("here is parent\n");
	}
	else
	{
		printf("Error in fork()");
	}
	return 0;
}
