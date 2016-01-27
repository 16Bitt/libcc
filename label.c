#include "libcc.h"

int lblcount = 0;

char* getlabel(){
	return dynstring("label%d", lblcount++);
}

void putlabel(char* label){
	emitln_notab("%s:", label);
}
