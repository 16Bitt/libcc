#include "libcc.h"
#include "stdio.h"
#include "ctype.h"

FILE* code_input;
unsigned char look;

void whitespace(){
	while(isblank(look))
		getcharacter();
}

void getcharacter(){
	int val = fgetc(code_input);
	if(val == EOF)
		look = 0xFF;
	else
		look = (unsigned char) val;
}

char* getnumber(){
	char buffer[BUFFSZ];
	int i = 0;

	if(!isdigit(look))
		expected("Number");

	while(isdigit(look)){
		buffer[i++] = look;
		getcharacter();
		if(i >= BUFFSZ - 1)
			error("Number exceeded max length");
	}

	buffer[i] = '\0';
	whitespace();

	return dynstring("%s", buffer);
}

char* getname(){
	char buffer[BUFFSZ];
	int i = 0;

	if(!isalnum(look))
		expected("Number");

	while(isalnum(look)){
		buffer[i++] = look;
		getcharacter();
		if(i >= BUFFSZ - 1)
			error("Name exceeded max length");
	}

	buffer[i] = '\0';
	whitespace();

	return dynstring("%s", buffer);
}

void match(char* sym){
	int i = 0;
	
	while(sym[i] != '\0'){
		if(sym[i++] != look){
			getcharacter();
			expected(sym);
		}
	}
}
