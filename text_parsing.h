#ifndef TEXT_PARSING_H
#define TEXT_PARSING_H

const int buffer_limit = 1024;
const int buffer_init = 64;




typedef struct target{
	char* name;
	char** dependecies;
	char** cmds;
}target;

typedef struct target_list{
	target** targets;
	int size;
}target_list;




#endif