#ifndef TEXT_PARSING_H
#define TEXT_PARSING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int const buffer_limit = 1024;
static int const buffer_init = 64;

//int num_targets;

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
	int deps_size;
	list_node* cmds;
	int cmds_size;
};

struct target_node{
	target* t;
	struct target_node* next;
};











#endif
