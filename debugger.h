
#ifndef _TEST_H_
#define _TEST_H_
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <stdint.h>

typedef struct
{
	char* prog;
	pid_t pid;
} debug;


void run(debug dbg);

#endif
