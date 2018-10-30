#ifndef TEXT_PARSING
#define TEXT_PARSING

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int buffer_limit = 1024;
const int buffer_init = 64;

int num_targets;

typedef struct list_node list_node;
typedef struct target target;
typedef struct target_node target_node;

struct list_node{
	char* val;
	struct list_node* next;

};

struct target{
	char* name;
	list_node* dependencies;
	list_node* cmds;
};

struct target_node{
	target* t;
	struct target_node* next;
};











#endif
