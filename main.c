#include <stdio.h>
#include <unistd.h>

#include <sys/ptrace.h>
#include <sys/types.h>

#include "debugger.h"

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
			printf("Error in child\n");
			return -1;
		}
		execl(argv[1], argv[1], NULL);
	}
	else if(pid >= 1)
	{
		debug dbg;
		dbg.prog = argv[1];
		dbg.pid = pid;
		printf("dbg.prog=%s, dbg.pid=%d\n", dbg.prog, dbg.pid);
		run(dbg);
	}
	else
	{
		printf("Error in fork()");
	}
	return 0;
}
