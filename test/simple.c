#include "libcc.h"
#include "ctype.h"

void operator();
void expression();
int scopedepth = 0;
char* current_type = "int";
bool global = true;

char* reduceptr(char* type){
	size_t len = strlen(type);
	char buff[BUFFSZ];

	if(type[len - 1] == '*'){
		int i;
		for(i = 0; i < len - 1; i++)
			buff[i] = type[i];
		buff[i++] = '\0';

		return dynstring("%s", buff);
	}

	else
		return type;
}

void settype(char* type){
	current_type = type;
}

void identifier(){
	char* name = getname();

	if(!idexists(name))
		error("Identifier '%s' is undeclared", name);


	if(look == '('){
		if(!isfunction(name))
			error("Cannot call variable '%s'", name);
		
		int len = 0;
		match("(");
		while(look != ')'){
			expression();
			emitln("push eax");
			if(look != ')')
				match(",");
			len++;
		}
		match(")");
		
		if(len != numargs(name))
			error("Call to %s with wrong number of args", name);

		emitln("call %s", getaccessor(name));
	}

	else {
		emitln("xor eax, eax");
		current_type = gettype(name);
		STRSWITCH(current_type)
			STRCASE("char")
				emitln("mov al, byte [%s]", getaccessor(name));
			STRCASE("short")
				emitln("mov ax, word [%s]", getaccessor(name));
			STRDEFAULT
				emitln("mov eax, dword [%s]", getaccessor(name));
		STRSWITCHEND

	}
}

void term(){
	if(look == '*'){
		match("*");
		term();
		emitln("xor ebx, ebx");
		settype(reduceptr(current_type));
		STRSWITCH(current_type)
			STRCASE("short")
				emitln("mov bx, word [eax]");
			STRCASE("char")
				emitln("mov bl, byte [eax]");
			STRDEFAULT
				emitln("mov ebx, dword [eax]");
		STRSWITCHEND
		emitln("xchg eax, ebx");
	} else if(look == '('){
		match("(");
		emitln("push eax");
		expression();
		match(")");
	}

	else if(look == '"'){
		emitln("mov eax, %s", add_string(getstring('"')));
	}

	else if(is_in(dynstring("%c", look), "+", "-", NULL)){
		emitln("push dword 0");
		operator();
	}

	else if(isalpha(look)){
		identifier();
	}

	else if(isdigit(look)){
		emitln("mov eax, %s", getnumber());
	}

	else
		expected("Number or variable");
}

void operator(){
	char op = look;

	emitln("pop ebx");

	match(dynstring("%c", op));
	if(op == '['){
		char* type = reduceptr(current_type);
		emitln("push ebx");
		expression();
		match("]");
		
		emitln("pop ebx");
		STRSWITCH(type)
			STRCASE("char")
			STRCASE("short")
				emitln("shl eax, 1");
			STRCASE("int")
				emitln("shl eax, 2");
		STRSWITCHEND

		  emitln("add eax, ebx");
		  emitln("xchg eax, ebx");
		  emitln("xor eax, eax");

		  STRSWITCH(current_type)
		  		STRCASE("int")
		  			emitln("mov eax, dword [ebx]");
		  		STRCASE("short")
		  			emitln("mov ax, word [ebx]");
		  		STRCASE("char")
		  			emitln("mov al, byte [ebx]");
		  		STRDEFAULT
		  			//error("Invalid type '%s'", current_type);
		  			emitln("mov eax, dword [ebx]");
		  STRSWITCHEND
	}
	else{
		term();

			  switch(op){
				  case '+':
					  emitln("add eax, ebx");
					  break;
				  case '-':
					  emitln("sub eax, ebx");
					  emitln("neg eax");
					  break;
				  case '*':
					  emitln("imul ebx");
					  break;
				  case '/':
					  emitln("xchg eax, ebx");
					  emitln("idiv ebx");
					  break;
				  default:
					  expected("Operator");
			  }
	}
}

void expression(){
	term();
	while(is_in(dynstring("%c", look), "[", "+", "-", "*", "/", NULL)){
		emitln("push eax");
		operator();
	}
}

void assignment(char* name){
	if(!idexists(name))
		error("Cannot assign to '%s', identifier undeclared", name);
	
	char* type = gettype(name);

	if(look == '['){
		match("[");
		expression();
		match("]");
		emitln("lea ebx, [%s]", getaccessor(name));
		STRSWITCH(reduceptr(type))
			STRCASE("char")
			STRCASE("short")
				emitln("shl eax, 1");
			STRDEFAULT
				emitln("shl eax, 2");
		STRSWITCHEND
		emitln("add ebx, eax");
		emitln("push ebx");
		
		match("=");
		expression();
		emitln("pop ebx");

		STRSWITCH(reduceptr(type))
			STRCASE("char")
				emitln("mov byte [ebx], al");
			STRCASE("short")
				emitln("mov word [ebx], ax");
			STRDEFAULT
				emitln("mov dword [ebx], eax");
		STRSWITCHEND
	} else {
		match("=");
		expression();
		emitln("mov dword [%s], eax", getaccessor(name));
	}

	match(";");
}

void block();

void doif(){
	char* label = getlabel();
	
	match("(");
	expression();
	emitln("or eax, eax");
	emitln("jz %s", label);
	match(")");

	block();

	if(strcmp(peekname(), "else") == 0){
		match("else");
		char* label2 = getlabel();
		emitln("jmp %s", label2);
		putlabel(label);
		block();
		putlabel(label2);
	}

	else
		putlabel(label);
}

void doasm(){
	emit("");
	while(look != ';'){
		emit_notab("%c", look);
		getcharacter();
	}
	emit("\n");
	match(";");
}

bool istype(char* name){
	return is_in(name, "void", "char", "short", "int", NULL);
}

char* gettypename(){
	char* name = getname();
	while(look == '*'){
		name = dynstring("%s*", name);
		match("*");
	}
	
	return name;
}

void dofunction(char* name, char* type){
	if(!global)
		error("Cannot define a function within another function");
	


	char* jumpover = getlabel();
	emitln("jmp %s", jumpover);

	global = false;

	int len = 0;
	char* buff[BUFFSZ];
	scopedepth = 0;
	
	//Get our variables
	match("(");
	while(look != ')'){
		buff[len++] = gettypename();
		buff[len++] = getname();
		if(look != ')')
			match(",");
	}
	match(")");
	
	//Start the scope and add our function
	addfunc(name, type, name, len / 2);
		
	if(look == ';'){
		emitln_notab("extern %s", name);
		match(";");
		global = true;
		putlabel(jumpover);
		return;
	}

	//Add variables to the scope
	startscope();
	int i;
	for(i = 0; i < len; i += 2)
		addvar(buff[i + 1], buff[i], dynstring("ebp + %d", ((len / 2) - (i / 2)) * 4 + 8));
	
	//Emit the prelude
	emitln("\nglobal %s", name);
	putlabel(name);
	emitln("push ebp");
	emitln("mov ebp, esp");
	
	if(strcmp(name, "main") == 0)
		emitln("call libcc_init");
	
	//Compile the block
	block();
	endscope();
	scopedepth = 0;
	
	//Emit the function end
	emitln("mov esp, ebp");
	emitln("pop ebp");
	emitln("ret"); 
	
	putlabel(jumpover);
	global = true;
}

void dowhile(){
	char* lblstart	= getlabel();
	char* lblend	= getlabel();
	
	putlabel(lblstart);
	match("(");
	expression();
	match(")");
	emitln("or eax, eax");
	emitln("jz %s", lblend);
	block();
	emitln("jmp %s", lblstart);
	putlabel(lblend);
}

void dovariable(char* name, char* type){
	if(!global){
		addvar(name, type, dynstring("ebp-%i", scopedepth + 4));
		scopedepth += 4;
		
		emitln("sub esp, 4");
		
		if(look == '=')
			assignment(name);
		else
			match(";");
	}

	else {
		char* lbl = getlabel();
		char* jumpover = getlabel();
		addvar(name, type, lbl);
		emitln("jmp %s", jumpover);
		emitln_notab("section .data");
		putlabel(lbl);
		STRSWITCH(type)
			STRCASE("char")
				emitln("db 0");
			STRCASE("short")
				emitln("dw 0");
			STRDEFAULT
				emitln("dd 0");
		STRSWITCHEND
		emitln_notab("section .text");
		putlabel(jumpover);
			
		if(look == '=')
			assignment(name);
		else
			match(";");
	}
}

void declare(char* name){
	char* type = name;
	while(look == '*'){
		type = dynstring("%s*", type);
		match("*");
	}
	
	char* realname = getname();
	if(look == '('){
		dofunction(realname, type);
	}
	
	else{
		dovariable(realname, type);
	}
}

void code(){
	char* name = peekname();
	STRSWITCH(name)
		STRCASE("if")
			match("if");
			doif();
		STRCASE("while")
			match("while");
			dowhile();
		STRCASE("return")
			if(global)
				error("Return outside of function");
			match("return");
			expression();
			emitln("mov esp, ebp");
			emitln("pop ebp");
			emitln("ret");
			match(";");
		STRCASE("asm")
			match("asm");
			doasm();
		STRDEFAULT
			if(isfunction(name)){
				term();
				match(";");
			}
		   else if(istype(name)){
				match(name);
		        	declare(name);
			}
		   else{
				match(name);
			        assignment(name);
			}
	STRSWITCHEND
}

void block(){
	match("{");
	while(look != '}'){
		code();
	}
	match("}");
}

void program(){
	while(look != 0xFF){
		char* name = peekname();
			if(istype(name)){
				match(name);
					declare(name);
			}
			else{
				match(name);
				assignment(name);
			}
	}
}
