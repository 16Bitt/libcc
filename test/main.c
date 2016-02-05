#include "stdio.h"
#include "libcc.h"

extern void program();

int main(int argc, char** argv){
	libcc_init(stdin, stdout, stderr);
	
	emitln_notab("section .text");
	putlabel("libcc_init");
	program();
	emitln("ret");
	
	emitln_notab("section .rodata");
	emit_strings("%s: db \"%s\", 0");

	return 0;
}
