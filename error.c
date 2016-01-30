#include "stdio.h"
#include "stdarg.h"
#include "libcc.h"
#include "stdlib.h"

FILE* error_output;

void warning(const char* fmt, ...){
	fprintf(error_output, "Warning:\t");
	
	va_list args;
	va_start(args, fmt);
	vfprintf(error_output, fmt, args);
	va_end(args);
	fputc('\n', error_output);
}

void error(const char* fmt, ...){
	fprintf(error_output, "Error:  \t");
	
	va_list args;
	va_start(args, fmt);
	vfprintf(error_output, fmt, args);
	va_end(args);
	fputc('\n', error_output);
	exit(-1);
}

void expected(char* s){
	error("Expected '%s' but look='%c'", s, look);
}
