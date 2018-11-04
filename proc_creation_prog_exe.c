#include "build_spec_graph.h"
#include "text_parsing.h"

/* This function checks if the node passed in exists as a target, prints an error to stderr if it does not 
and returns null.  Once the node is found, it calls the run commands function which runs commands bottom-up from the subgraph of
current node
*/
void makeTargets(char* name, graph_node_list* gnl){
    graph_node* curr_graph_node = NULL;
    graph_node_list* curr_gnle = gnl;

    //find the addr of the node we need to build
    while(curr_gnle != NULL){
        //check for matching name
        if(strcmp(curr_gnle->name,name) == 0){
            //check that it is a target, ie, the matching node contains a target node
            if(curr_gnle->addr->gnt != NULL){
                curr_graph_node = curr_gnle->addr;
                //once we've met these conditions, we break
                break;
            }  
        }
    }
    //if curr_graph_node stays null, then we never found the target in our graph_node_list
    if(curr_graph_node == NULL){
        fprintf(stderr,"%s is not a valid target in this makefile",name);
        return NULL;
    }




}

void dfs(graph_node* node){
    //check if nodes have children, for each child, recurse
    if(node->gnt->deps_size > 0){
        for(int i = 0; i < deps_size; i++){
            dfs(node->children[i]);
        }

    }else{
        runCommands(node);
    }


}

runCommands(graph_node* node){

}


