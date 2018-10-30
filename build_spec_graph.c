#include "build_spec_graph.h"
extern graph_node_list* start;
extern graph_node_list* end;

/*
This function will create a graph node that has a non-NULL target_node pointer, this means this graph_node is a target and will have dependencies
This function does not add the dependencies as children for the graph node, that will be done later when all graph_nodes for targets and non-targets are made
*/
graph_node* create_target_graph_node(target_node* curr_target_node){
    graph_node* gn = malloc(sizeof(graph_node));
    gn->name = curr_target_node->t->name;
    gn->gnt = curr_target_node->t;
    gnl_size++;
    return gn;
}

/* 
This function will create and return a graph_node_list, creates a start node that is empty, initially start and end global variables both point to this
node.  Use start and end to check if they are equal to each other.  If start has a null name field, then the start node is empty and add to it.
*/
graph_node_list* create_graph_node_list(){
    graph_node_list* new = malloc(sizeof(graph_node_list));
    new->name = NULL;
    start = new;
    end = new;
    return new;
}

/* 
This function will add a graph node to the graph node list.
*/
void add_graph_node(graph_node* gn){
    
    //check if start name = NULL, this will tell us whether our list is currently empty and start = end.
    if (start->name == NULL){
        start->name = gn->name;
        start->addr = gn;
        start->next = NULL;
        /*
        Don't add a next to it, we now have start and end pointing to the beginning node, once we 
        add nodes from here on out, we will create a new graph_node_list node and make ends next equal to it
        set end to NULL so we make sure there are no junk values that throw us off
        */
    }
    /*
    Assume now that we do not have an empty list, therefore we use this logic:
    1. create new graph_node_list pointer
    2. have end->next point to it and move end to end->next
    3. set name of new graph_node_list to gn name, set addr = gn, set next = NULL

    setting end/new_gnl_node ->next = NULL makes sure we have a NULL value for next at the end of the gnl when iterating it.
    */
   //1.
   graph_node_list* new_gnl_node = malloc(sizeof(graph_node_list));
   //2.
   end->next = new_gnl_node;
   end = end->next;
   //3.
   new_gnl_node->name = gn->name;
   new_gnl_node->addr = gn;
   new_gnl_node->next = NULL;
}
/* 
This function returns true if the passed in name exists as a graph node in the gnl
*/
int exists_in_graph_node_list(graph_node_list* gnl, char* name){
    graph_node_list* curr = gnl;
    while(curr != NULL){
        if(strcmp(curr->name, name) == 0){
            //found a match, let's move on
            return 1;
        }else{
            curr = curr->next;
        }
    }
    return 0;
}


/*
this function will build a target node list for adding children to target nodes, use this
once to get addresses to all the nodes.  When adding children to target nodes, check this list for addr
to specific node. Logic of this function:
1. Create a new graph_node_list
2. First pass: traverse target nodes, creating graph nodes and adding them to the graph_node_list
3. Second pass: traverse graph_node_list, for each graph_node in list, check it's target's dependencies, if any dependency is not in the graph_node_list
    add it to graph node list, these will be graph_nodes that have NULL pointers to target's since the graph_node isn't a target, just a dependency
4. Now graph_node_list has all target graph_nodes and non-target graph_nodes, use this list to create the dependency graph.
*/
graph_node_list* build_graph_node_list(target_node* curr_target_node){
    if(curr_target_node == NULL){
        //target list is empty return null
        return NULL;
    }
    //1.
    graph_node_list* gnl = create_graph_node_list();

    //2. First pass
    while(curr_target_node != NULL){
        graph_node* new_graph_node = create_target_graph_node(curr_target_node);
        //add to graph_node list
        add_graph_node(new_graph_node);
        curr_target_node = curr_target_node->next;
    }

    //3. Second pass
    while(gnl != NULL){
        //isolate the target we are looking at
        target* curr_target = gnl->addr->gnt;

        //isolate the list of dependencies for that target
        list_node* curr_dep = curr_target->dependencies;

        while(curr_dep != NULL){
            //if the current dependency we are examining does not exist in the gnl, create a new graph node and add it
            if(!exists_in_graph_node_list(curr_dep->val)){
                
            }
            curr_dep = curr_dep->next;
        }
    }

    //iterate through
    return NULL;

}


int main(){
    return 1;
}