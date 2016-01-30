#include "libcc.h"
#include "ctype.h"

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
		
		if(len != numargs(num))
			error("Call to %s with wrong number of args", name);

		emitln("call %s", getaccessor(name));
	}

	else {
		emitln("mov eax, %s", name);
	}
}

void operator();

void term(){
	if(is_in(dynstring("%c", look), "+", "-", NULL)){
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
	emitln("mov dword [%s], eax", name);
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

void code(){
	char* name = getname();
	STRSWITCH(name)
		STRCASE("if")
			doif();
		STRCASE("var")
		STRCASE("while")
		STRCASE("return")
		STRCASE("asm")
			doasm();
		STRDEFAULT
			assignment(name);
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
