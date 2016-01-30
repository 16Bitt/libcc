#include "libcc.h"
#include "stdio.h"
#include "ctype.h"

FILE* code_input;
unsigned char look;

void whitespace(){
	while(isspace(look))
		getcharacter();
}

void getcharacter(){
	look = fgetc(code_input);
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
		expected("Name");

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
	int len = strlen(sym);
	int i;

	for(i = 0; i < len; i++){
		if(sym[i] != look){
			expected(sym);
		}
		
		getcharacter();
	}

	whitespace();
}

char* peekname(){
	char buff[BUFFSZ];
	int i = 0;

	//Get the next token from the file stream
	while(isalnum(look)){
		buff[i++] = look;
		if(i >= BUFFSZ - 1)
			error("Token exceeded maximum length");
		getcharacter();
	}
	buff[i] = '\0';

	char* ret = dynstring("%s", buff);
	
	//Pack the data back into the stream
	buff[i] = look;
	while(i >= 0)
		ungetc(buff[i--], code_input);
	
	getcharacter();
	return ret;
}
