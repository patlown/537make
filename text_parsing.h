#ifndef TEXT_PARSING_H
#define TEXT_PARSING_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int buffer_limit = 1024;
const int buffer_init = 64;

typedef struct list_node{
	char* val;
	struct list_node* next;

}list_node;

typedef struct{
	char* name;
	list_node* dependencies;
	list_node* cmds;
}target;

typedef struct target_node{
	target* t;
	struct target_node* next;
}target_node;











#endif
