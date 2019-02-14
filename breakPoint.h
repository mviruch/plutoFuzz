#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	pid_t pid;
	intptr_t addr;
	bool enable;
	uint8_t saveData;
	int id;
} breakPoint;
