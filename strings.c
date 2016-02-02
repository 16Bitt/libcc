#include "libcc.h"
#include "stdlib.h"

typedef struct string_saver{
	char* label;
	char* string;
	struct string_saver* next;
} string_saver_t;

string_saver_t* strings_root = NULL;
int string_i = 0;

void strings_init(){
	strings_root = (string_saver_t*) malloc(sizeof(string_saver_t));
	strings_root->label = "LIBRARY_STR";
	strings_root->string = "Made with libcc";
	strings_root->next = NULL;
}

char* add_string(char* str){
	string_saver_t* ss = (string_saver_t*) malloc(sizeof(string_saver_t));
	ss->next = strings_root->next;
	strings_root->next = ss;
	
	ss->label = getlabel();
	ss->string = str;

	return ss->label;
}

void emit_strings(char* fmt){
	string_saver_t* current = strings_root;
	while(current != NULL){
		emitln(fmt, current->label, current->string);
		current = current->next;
	}
}
