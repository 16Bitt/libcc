#ifndef LIBCC_H
#define LIBCC_H

#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdbool.h"

#define BUFFSZ	256

//Our lookahead character
extern unsigned char look;

//Target files
extern FILE* code_output;
extern FILE* code_input;
extern FILE* error_output;

//Query the input stream
void getcharacter();
char* getnumber();
char* getname();

//Check for a specific token
void match(char* sym);

//Error handling
void warning(const char* fmt, ...);
void error(const char* fmt, ...);
void expected(char* s);

//Code output
void emit(const char* fmt, ...);
void emitln(const char* fmt, ...);
void emitln_notab(const char* fmt, ...);

//Label handling
char* getlabel();
void putlabel(char* lbl);

//Utility function
bool is_in(char* str, ...);
char* dynstring(const char* str, ...);

//Switch case for strings...
#define STRSWITCH(val) char* tmpswitchval = (val); if(false){}
#define STRCASE(val) else if(strcmp(tmpswitchval, (val)) == 0){
#define STRCASEEND }

//Initialization
void libcc_init(FILE* in, FILE* out, FILE* err);

#endif
