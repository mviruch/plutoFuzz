#include <stdio.h>
#include <stdarg.h>

#include <sys/types.h>

void msg(pid_t pid, const char* format, ...)
{
	va_list ap;
	fprintf(stdout, "[%d] ", pid);
	va_start(ap, format);
	vfprintf(stdout, format, ap);
	va_end(ap);
	printf("\n");
}
