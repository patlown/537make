#ifndef BUILD_SPEC_GRAPH_H
#define BUILD_SPEC_GRAPH_H

#include "text_parsing.h"



/* This structure will point to target_node for its data and will graph_nodes as its children
*/
typedef struct graph_node{
	char* name;
	target* gnt;
	graph_node** children;

}graph_node;

/* This structure stores names,addrs of all graph nodes, use it to search if a graph node exists and get its addr
*/
typedef struct graph_node_list{
	char *name;
	graph_node* addr;
	graph_node_list* next;
}graph_node_list;

//this holds info on the size of the graph_node_list
int gnl_size = 0;

//hold pointers to start and end of list
graph_node_list* start;
graph_node_list* end;






#endif