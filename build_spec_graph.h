#ifndef BUILD_SPEC_GRAPH_H
#define BUILD_SPEC_GRAPH_H

#include "text_parsing.h"

typedef struct{

	target_node* tn;
	target_node** children;

}graph_node;

typedef struct target_node_list{
	char *name;
	target_node* addr;
	target_node* next;
}target_node_list;








#endif BUILD_SPEC_GRAPH_H