#ifndef BUILD_SPEC_GRAPH
#define BUILD_SPEC_GRAPH

#include "text_parsing.h"

typedef struct graph_node graph_node;
typedef struct graph_node_list graph_node_list;


/* This structure will point to target_node for its data and will graph_nodes as its children
*/
struct graph_node{
	char* name;
	target* gnt;
	graph_node** children;

};

/* This structure stores names,addrs of all graph nodes, use it to search if a graph node exists and get its addr
*/
struct graph_node_list{
	char *name;
	graph_node* addr;
	graph_node_list* next;
};

//this holds info on the size of the graph_node_list
int gnl_size = 0;

//hold pointers to start and end of list
graph_node_list* start;
graph_node_list* end;






#endif