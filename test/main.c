#include "stdio.h"
#include "libcc.h"

extern void program();

int main(int argc, char** argv){
	libcc_init(stdin, stdout, stderr);
	
	program();
	
	emit_strings("%s: \"%s\", 0");

	return 0;
}
