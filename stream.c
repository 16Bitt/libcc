#include "libcc.h"
#include "stdio.h"
#include "ctype.h"

FILE* code_input;
unsigned char look;
int line_no = 1;
bool libcc_eof = false;

void whitespace(){
	while(isspace(look)){
		if(look == '\n')
			line_no++;
		getcharacter();
		
		//Check for EOF
		if(libcc_eof)
			break;
	}
}

void getcharacter(){
	int val = fgetc(code_input);
	if(val == EOF)
		libcc_eof = true;
	else
		look = val;
}

char getsomething(){
	whitespace();
	char ret = look;
	getcharacter();
	return ret;
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

char* getstring(char delim){
	char buff[BUFFSZ];
	int i = 0;

	match(dynstring("%c", delim));
	while(look != delim){
		buff[i++] = look;
		getcharacter();
		if(i >= BUFFSZ - 1)
			error("String exceeded maximum length");
	}
	buff[i] = '\0';
	match(dynstring("%c", delim));

	whitespace();
	return dynstring("%s", buff);
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
		
		//Check for the end of file
		if(libcc_eof)
			break;
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
