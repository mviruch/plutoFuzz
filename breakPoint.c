#include "breakPoint.h"

#include "msg.h"

#include <sys/user.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#include <stdint.h>
#include <stdbool.h>

void enable(breakPoint *bp)
{
	long data = ptrace(PTRACE_PEEKTEXT, bp->pid, bp->addr, NULL);
	bp->saveData = data;
	long int3 = (data & 0xFFFFFFFFFFFFFF00)|0xcc;
	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, bp->pid, 0, &regs);
	ptrace(PTRACE_POKETEXT, bp->pid, bp->addr, int3);

	bp->enable = true;
	data = ptrace(PTRACE_PEEKTEXT, bp->pid, bp->addr, 0);
	
	msg(bp->pid, "breakPoint at 0x%08x: 0x%8lx enable.", bp->addr, 
	ptrace(PTRACE_PEEKTEXT, bp->pid, bp->addr, 0));
}

void disable(breakPoint *bp)
{
	long data = ptrace(PTRACE_PEEKTEXT, bp->pid, bp->addr, NULL);
	long reData = (bp->saveData & 0xFFFFFFFFFFFFFF00)|data;
	ptrace(PTRACE_POKETEXT, bp->pid, bp->addr, bp->saveData);
	data = ptrace(PTRACE_PEEKTEXT, bp->pid, bp->addr, NULL);
	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, bp->pid, 0, &regs);
	msg(bp->pid, "breakPoint at 0x%08x: 0x%llx disable.", bp->addr, data);
	bp->enable = false;
}

void resume(breakPoint *bp)
{
	//first put the right data
	disable(bp);
	//then set eip/rip
	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, bp->pid, 0, &regs);
	//regs.rip -= 1;
	regs.rip = bp->addr;
	ptrace(PTRACE_SETREGS, bp->pid, 0, &regs);

	//make sure eip/rip is rewrite
	struct user_regs_struct regg;
	ptrace(PTRACE_GETREGS, bp->pid, 0, &regg);

	//step
	ptrace(PTRACE_SINGLESTEP, bp->pid, 0, 0);
	wait(0);

	//enable
	enable(bp);
}

bool isEnable(breakPoint *bp)
{
	return bp->enable;
}

intptr_t getAddr(breakPoint *bp)
{
	return bp->addr;
}
