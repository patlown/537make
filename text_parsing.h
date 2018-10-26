#ifndef TEXT_PARSING_H
#define TEXT_PARSING_H

const int buffer_limit = 1024;
const int buffer_init = 64;


typedef struct{
	target** targets;
	int size;
}target_list;

typdef struct{
	char* name;
	char** dependecies;
	char** cmds;
}target;




#endif