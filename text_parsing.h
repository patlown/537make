#ifndef TEXT_PARSING_H
#define TEXT_PARSING_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int buffer_limit = 1024;
const int buffer_init = 64;

typedef struct{
	char* name;
	char** dependecies;
	char** cmds;
}target;

typedef struct{
	target** targets;
	int size;
}target_list;






#endif