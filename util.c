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

bool is_in(char* str, ...){
	va_list args;
	va_start(args, str);
	
	char* argument = va_arg(args, char*);
	while(argument != NULL){
		if(strcmp(str, argument) == 0){
			va_end(args);
			return true;
		}

		argument = va_arg(args, char*);
	}

	va_end(args);
	return false;
}
