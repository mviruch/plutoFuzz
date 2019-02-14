#include <sys/types.h>
#include <sys/wait.h>
#include "debugger.h"
#include "linenoise/linenoise.h"
#include "linenoise/linenoise.c"
#include <string.h>
#include "breakPoint.c"
#include <stdlib.h>

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

void contDebug(debug dbg)
{
	ptrace(PTRACE_CONT, dbg.pid, NULL, NULL);
	int waitStatus;
	int options = 0;
	waitpid(dbg.pid, &waitStatus, options);
}

void handleCommand(char* line, debug dbg)
{
	char dst[16][64];
	int cnt = split(dst, line, " ");
	if(strcmp(dst[0], "cont") == 0)
	{
		contDebug(dbg);
	}
	else if(strcmp(dst[0], "bp") == 0)
	{
		char *endPtr;
		breakPoint bp;
		bp.pid = dbg.pid;
		bp.addr = strtoul(dst[1], &endPtr, 16);
		bp.id = bpId;
		bPool[bpId] = bp;
		printf("0x%08x\n%s\n", bp.addr, dst[1]);
		enable(bPool[bpId]);
		bpId++;
	}
}

void run(debug dbg)
{
	int waitStatus;
	int options = 0;
	char* line = NULL;
	waitpid(dbg.pid, &waitStatus, options);
	while((line = linenoise("dbg> ")) != NULL)
	{
		handleCommand(line, dbg);
		linenoiseHistoryAdd(line);
		linenoiseFree(line);
	}
}
