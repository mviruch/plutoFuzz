#include "breakPoint.h"
#include <sys/types.h>
#include <sys/ptrace.h>
#include <stdint.h>
#include <stdbool.h>

void enable(breakPoint bp)
{
	//TODO
	unsigned data = ptrace(PTRACE_PEEKTEXT, bp.pid, bp.addr, NULL);
	printf("Original data at 0x%08x: 0x%08x\n", bp.addr, data);
	
	bp.saveData = data;

	unsigned int3 = (data & 0xFFFFFF00)|0xcc;

	printf("0x%08x\n", int3);
	ptrace(PTRACE_POKETEXT,
			bp.pid,
			bp.addr,
			int3);
	bp.enable = true;
	data = ptrace(PTRACE_PEEKTEXT, bp.pid, bp.addr, 0);
	printf("Now data at 0x%08x: 0x%08x\n", bp.addr, data);
}

void disable(breakPoint bp)
{
	//TODO
	unsigned data = ptrace(PTRACE_PEEKTEXT, bp.pid, bp.addr, 0);
	unsigned reData = (data & 0xFFFFFF00)|data;
	ptrace(PTRACE_POKEDATA, bp.pid, bp.addr, reData);
	bp.enable = false;

}

bool isEnanle(breakPoint bp)
{
	return bp.enable;
}

uint8_t getAddr(breakPoint bp)
{
	return bp.addr;
}
