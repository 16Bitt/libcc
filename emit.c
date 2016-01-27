#include "stdio.h"
#include "libcc.h"
#include "stdarg.h"

FILE* code_output;

void emit(const char* fmt, ...){
	fprintf(code_output, "\t");
	va_list args;
	va_start(args, fmt);
	vfprintf(code_output, fmt, args);
	va_end(args);
}

void emitln(const char* fmt, ...){
	fprintf(code_output, "\t");
	va_list args;
	va_start(args, fmt);
	vfprintf(code_output, fmt, args);
	va_end(args);
	putchar('\n');
}

void emitln_notab(const char* fmt, ...){
	fprintf(code_output, "\t");
	va_list args;
	va_start(args, fmt);
	vfprintf(code_output, fmt, args);
	va_end(args);
	putchar('\n');
}
