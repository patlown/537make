/*
Authors: Patrick Lown and Ge Xu
*/
#ifndef TEXT_PARSING_H
#define TEXT_PARSING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//max limit for line length & initial line buffer size
static int const buffer_limit = 1024;
static int const buffer_init = 64;


typedef struct list_node list_node;
typedef struct target target;
typedef struct target_node target_node;

/* This structure works as a linked list, store a string ptr and a ptr to nxt node
** Use this struct to store dependencied and cmds in target struct
*/
struct list_node{
	char* val;
	struct list_node* next;

};

/* This structure stores names,dependencies and count,commands and counts every target
*/
struct target{

	char* name;
	list_node* dependencies;
	int deps_size;
	list_node* cmds;
	int cmds_size;
};

/* This structure works as a linked list that contains a target and a ptr to next node
*/
struct target_node{
	target* t;
	struct target_node* next;
};

target_node* parseFile(char * filename);
char* double_buff(char* buff, int cursize);
void skip_empty(int* c,FILE* fp);
char* read_line(int* c, FILE* fp);
target* get_target(int *c, FILE* fp);
int valid_cmd(char* s);
int valid_target(char* s);
void printt(target *t);







typedef struct target_list{
	target** targets;
	int size;
}target_list;




#endif
