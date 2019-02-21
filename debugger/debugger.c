#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/user.h>

#include "../linenoise/linenoise.h"
#include "../linenoise/linenoise.c"

#include "debugger.h"
#include "../breakpoint/breakPoint.c"


int bpId = 0;
breakPoint bPool[64];

int split(char dst[][64], char* line, const char* spl)
{
	int n = 0;
	char *result = NULL;
	result = strtok(line, spl);
	while(result != NULL)
	{
		strcpy(dst[n++], result);
		result = strtok(NULL, spl);
	}
	return n;
}

void setBp(debug dbg, char dst[])
{
	breakPoint bp;
	char *endPtr;
	bp.pid = dbg.pid;
	bp.addr = strtoul(dst, &endPtr, 16);
	enable(&bp);
	bp.id = bpId;
	bPool[bpId] = bp;
	bpId++;
}

intptr_t getRip(pid_t pid)
{
	intptr_t rip;
	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, pid, 0, &regs);
	rip = regs.rip;
	return rip;
}

void contDebug(debug dbg, int* waitStatus)
{
	ptrace(PTRACE_CONT, dbg.pid, NULL, NULL);
	int waitStatuss;
	int options = 0;
	wait(&waitStatuss);
	if(WIFSTOPPED(waitStatuss))
	{
		if(waitStatuss == 1407)		//1407 is Trace/breakpoint trap
		{
			intptr_t rip = getRip(dbg.pid)-1;
			for(int i=0; i<bpId; i++)
			{
				if(rip == getAddr(&bPool[i]))
				{
					resume(&bPool[i]);
				}
			}
		}
	}
}

void printInfo(pid_t pid)
{
	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, pid, 0, &regs);
	printf("rip: 0x%08llx, rax: 0x%08llx, rbx: 0x%08llx\nrcx: 0x%08llx, rdx: 0x%08llx, rsi: 0x%08llx\nrdi: 0x%08llx, rbp: 0x%08llx, rsp: 0x%08llx",
			regs.rip, regs.rax, regs.rbx, regs.rcx, regs.rdx, regs.rsi, regs.rdi, regs.rbp, regs.rsp);
}

void handleCommand(char* line, debug dbg, int* waitStatus)
{
	char dst[16][64];
	int cnt = split(dst, line, " ");
	if(strcmp(dst[0], "cont") == 0)
	{
		contDebug(dbg, waitStatus);
	}
	else if(strcmp(dst[0], "bp") == 0)
	{
		setBp(dbg, dst[1]);
	}
	else if(strcmp(dst[0], "s") == 0)
	{
		printInfo(dbg.pid);
	}
	else if(strcmp(dst[0], "bl") == 0)
	{
		printf("breakPoint list:\n");
		printf("#\t| addr\t\t| enable\n");
		for(int i=0; i<= bpId; i++)
		{
			printf("%d\t| 0x%08lx\t| %d\n", i, getAddr(&bPool[i]), isEnable(&bPool[i]));
		}
	}
}

void run(debug dbg)
{
	int waitStatus;
	int options = 0;
	char* line = NULL;

	wait(&waitStatus);

	while((line = linenoise("dbg> ")) != NULL)
	{		
		//printInfo(dbg.pid);
		handleCommand(line, dbg, &waitStatus);
		linenoiseHistoryAdd(line);
		linenoiseFree(line);
	}
}
