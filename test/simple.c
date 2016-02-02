#include "libcc.h"
#include "ctype.h"

void operator();
void expression();
int scopedepth = 0;

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
		emitln("mov eax, dword [%s]", getaccessor(name));
	}
}

void term(){
	if(look == '('){
		match("(");
		emitln("push eax");
		expression();
		match(")");
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

void expression(){
	term();
	while(is_in(dynstring("%c", look), "+", "-", "*", "/", NULL)){
		emitln("push eax");
		operator();
	}
}

void assignment(char* name){
	match("=");
	expression();
	emitln("mov dword [%s], eax", getaccessor(name));
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
	return is_in(name, "char", "short", "int", NULL);
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
		match(";");
		return;
	}

	//Add variables to the scope
	startscope();
	int i;
	for(i = 0; i < len; i += 2)
		addvar(buff[i + 1], buff[i], dynstring("ebp + %d", (len - i) * 4 + 4));
	
	//Emit the prelude
	putlabel(name);
	emitln("push ebp");
	emitln("mov ebp, esp");
	
	//Compile the block
	block();
	endscope();
	scopedepth = 0;
	
	//Emit the function end
	emitln("mov esp, ebp");
	emitln("pop ebp");
	emitln("ret"); 
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
	addvar(name, type, dynstring("ebp-%i", scopedepth));
	scopedepth += 4;
	
	emitln("sub esp, 4");
	
	if(look == '=')
		assignment(name);
	else
		match(";");
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
			match("if");
			dowhile();
		STRCASE("return")
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
		char* name = getname();
		STRSWITCH(name)
			STRCASE("def")
			STRCASE("var")
		STRSWITCHEND
	}
}
