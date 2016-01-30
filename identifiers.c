#include "libcc.h"
#include "stdlib.h"

typedef struct identifier{
	char*	type;
	char*	name;
	char* 	accessor;
	bool	function;
	bool	scope;
	int	num_args;
	struct	identifier* next;
} identifier_t;

identifier_t* id_root;

void identifiers_init(){
	id_root = (identifier_t*) malloc(sizeof(identifier_t));
	id_root->type = "NONE";
	id_root->name = "ROOT_RESERVED";
	id_root->next = NULL;
}

identifier_t* lookup(char* name){
	identifier_t* current = id_root;
	while(current != NULL){
		if(strcmp(name, current->name) == 0)
			return current;
		current = current->next;
	}

	return NULL;
}

bool idexists(char* name){
	return lookup(name) != NULL;
}

bool isfunction(char* name){
	identifier_t* id = lookup(name);
	if(id == NULL)
		return false;
	
	return id->function;
}

char* getaccessor(char* name){
	identifier_t* id = lookup(name);
	if(id == NULL)
		return NULL;

	return id->accessor;
}

char* gettype(char* name){
	identifier_t* id = lookup(name);
	if(id == NULL)
		return NULL;

	return id->type;
}

void startscope(){
	identifier_t* id = (identifier_t*) malloc(sizeof(identifier_t));
	memset(id, 0, sizeof(identifier_t));
	
	id->name	= "STARTING_NEW_SCOPE";
	id->type	= "";
	id->accessor	= "";
	id->function	= false;
	id->scope	= true;

	id->next 	= id_root->next;
	id_root->next	= id->next;
}

void addvar(char* name, char* type, char* accessor){
	identifier_t* id = (identifier_t*) malloc(sizeof(identifier_t));
	memset(id, 0, sizeof(identifier_t));
	
	id->name	= name;
	id->type	= type;
	id->accessor	= accessor;
	id->function	= false;
	id->scope	= false;

	id->next 	= id_root->next;
	id_root->next	= id->next;
}

void addfunc(char* name, char* type, char* accessor, int num_args){
	identifier_t* id = (identifier_t*) malloc(sizeof(identifier_t));
	memset(id, 0, sizeof(identifier_t));
	
	id->name	= name;
	id->type	= type;
	id->accessor	= accessor;
	id->function	= true;
	id->num_args	= num_args;
	id->scope	= false;

	id->next 	= id_root->next;
	id_root->next	= id->next;
}

void endscope(){
	identifier_t* current = id_root->next;
	while(current->scope != true){
		identifier_t* next = current->next;
		free(current);
		current = next;
		id_root->next = current;
	}

	id_root->next = current->next;
	free(current);
}

int numargs(char* name){
	identifier_t* id = lookup(name);
	if(id == NULL)
		return 0;

	return id->num_args;
}
