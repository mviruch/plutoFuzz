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
	//msg(dbg.pid, "rip in contDbg 0x%llx", getRip(dbg.pid));
	unsigned data = ptrace(PTRACE_PEEKTEXT, dbg.pid, getRip(dbg.pid), NULL);
	//msg(dbg.pid, "Data in contDbg at 0x%08xx: 0x%llx\n", getRip(dbg.pid), data);
	if(WIFSTOPPED(waitStatuss))
	{
		if(waitStatuss == 1407)		//1407 is Trace/breakpoint trap
		{
			intptr_t rip = getRip(dbg.pid)-1;
			for(int i=0; i<bpId; i++)
			{
				if(rip == bPool[i].addr)
				{
					//printf("rip bef resme 0x%08x\n", bPool[i].addr);
					//printf("saveData bef re 0x%08x\n", bPool[i].saveData);
					resume(&bPool[i]);
				}
			}
		}
	}
}

void handleCommand(char* line, debug dbg, int* waitStatus)
{
	char dst[16][64];
	int cnt = split(dst, line, " ");
	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, dbg.pid, 0, &regs);
	/*printf("rip= 0x%08x, instr= 0x%08x\n", regs.rip,
	ptrace(PTRACE_PEEKTEXT, dbg.pid, regs.rip, NULL)
	);
	msg(dbg.pid, "rip= 0x%08x, instr= 0x%8lx", regs.rip,
	ptrace(PTRACE_PEEKTEXT, dbg.pid, regs.rip, NULL));
	*/
	if(strcmp(dst[0], "cont") == 0)
	{
		contDebug(dbg, waitStatus);
	}
	else if(strcmp(dst[0], "bp") == 0)
	{
		setBp(dbg, dst[1]);
	}
}

void run(debug dbg)
{
	int waitStatus;
	int options = 0;
	char* line = NULL;

	wait(&waitStatus);
	//0x00400e06
	

	while((line = linenoise("dbg> ")) != NULL)
	{		
		handleCommand(line, dbg, &waitStatus);
		linenoiseHistoryAdd(line);
		linenoiseFree(line);
	}
}
