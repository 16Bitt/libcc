#ifndef LIBCC_H
#define LIBCC_H

#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdbool.h"

#define BUFFSZ	256

//Our lookahead character
extern unsigned char look;

//Current line we're parsing
extern int line_no;

//Target files
extern FILE* code_output;
extern FILE* code_input;
extern FILE* error_output;

//Query the input stream
void getcharacter();
char* getnumber();
char* getname();
char* peekname();
char* getstring(char delim);

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
void emit_notab(const char* fmt, ...);

//Label handling
char* getlabel();
void putlabel(char* lbl);

//Utility function
bool is_in(char* str, ...);
char* dynstring(const char* str, ...);

//Switch case for strings...
#define STRSWITCH(val) { char* tmpswitchval = (val); if(false){
#define STRCASE(val) }else if(strcmp(tmpswitchval, (val)) == 0){
#define STRSWITCHEND } }
#define STRDEFAULT }else{

//Initialization
void libcc_init(FILE* in, FILE* out, FILE* err);
void identifiers_init();

//Identifier handling
bool isfunction(char* name);
char* getaccessor(char* name);
char* gettype(char* name);
void startscope();
void addvar(char* name, char* type, char* accessor);
void addfunc(char* name, char* type, char* accessor, int num_args);
void endscope();
bool idexists(char* name);
int numargs(char* name);

//Strings
void strings_init();
char* add_string(char* str);
void emit_strings(char* fmt);

#endif
