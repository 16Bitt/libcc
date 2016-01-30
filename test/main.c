#include "stdio.h"
#include "libcc.h"

extern void block();

int main(int argc, char** argv){
	libcc_init(stdin, stdout, stderr);
	
	block();

	return 0;
}
