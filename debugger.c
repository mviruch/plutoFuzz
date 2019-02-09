#include <sys/types.h>
#include <sys/wait.h>
#include "debugger.h"
#include "linenoise/linenoise.h"
#include "linenoise/linenoise.c"
#include <string.h>

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
