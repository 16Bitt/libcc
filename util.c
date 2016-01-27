#include "stdio.h"
#include "libcc.h"
#include "stdlib.h"

char* dynstring(const char* fmt, ...){
	char* buffer = (char*) malloc(sizeof(char) * BUFFSZ);
	memset(buffer, 0, sizeof(char) * BUFFSZ);

	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	va_end(args);

	return buffer;
}
