#include "libcc.h"
#include "stdio.h"

void libcc_init(FILE* in, FILE* out, FILE* err){
	code_input = in;
	code_output = out;
	error_output = err;
}
