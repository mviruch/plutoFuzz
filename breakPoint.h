#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct
{
	pid_t pid;
	intptr_t addr;
	bool enable;
	long saveData;
	int id;
} breakPoint;

#ifndef _TEST_H_
#define _TEST_H_
void enable(breakPoint *bp);
void disable(breakPoint *bp);
void resume(breakPoint *bp);
bool isEnable(breakPoint *bp);
intptr_t getAddr(breakPoint *bp);

#endif
