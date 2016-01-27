#include "stdio.h"
#include "libcc.h"

int main(int argc, char** argv){
	libcc_init(stdin, stdout, stderr);
	
	warning("Test warning.");
	error("Test error.");

	return 0;
}
