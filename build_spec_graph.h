#ifndef BUILD_SPEC_GRAPH_H
#define BUILD_SPEC_GRAPH_H

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




//hold pointers to start and end of list
graph_node_list* start;
graph_node_list* end;

graph_node* create_target_graph_node(target_node* curr_target_node);
graph_node* create_non_target_graph_node(list_node* curr_dep);
graph_node_list* create_graph_node_list();
void add_graph_node(graph_node* gn);
int exists_in_graph_node_list(graph_node_list* gnl, char* name);
graph_node_list* build_graph_node_list(target_node* curr_target_node);
graph_node** alloc_graph_node_children(int size);
graph_node* get_graph_node(graph_node_list* gnl, char* name);
void build_dependency_graph(graph_node_list* gnl);
int is_DAG(graph_node* gnode,char* name);
int check_no_cycle(graph_node_list* gnl);




#endif