#include "build_spec_graph.h"
extern graph_node_list* start;
extern graph_node_list* end;
graph_node_list* check_list;
int check;
//this holds info on the size of the graph_node_list
int gnl_size = 0;


/*
This function will create a graph node that has a non-NULL target_node pointer, this means this graph_node is a target and will have dependencies
This function does not add the dependencies as children for the graph node, that will be done later when all graph_nodes for targets and non-targets are made
*/
graph_node* create_target_graph_node(target_node* curr_target_node){
    graph_node* gn = malloc(sizeof(graph_node));
    gn->name = curr_target_node->t->name;
    gn->gnt = curr_target_node->t;
    //set this to NULL for easy check whether children have been allocated
    gn->children = NULL;
    return gn;
}

/*
This function will create a graph node with a NULL target_node pointer, pass it a dependency list element and it will create a graph node with the same name.
This does not add children to the graph_nodes, that will be handled on a graph_node by graph_node basis after the graph_node_list is created.
*/
graph_node* create_non_target_graph_node(list_node* curr_dep){
    graph_node* gn = malloc(sizeof(graph_node));
    gn->name = curr_dep->val;
    //wipe any junk values that gnt may be pointing to
    gn->gnt = NULL;
    //set this to NULL since this has no dependencies/children in graph, don't want to accidentally read junk values
    gn->children = NULL;
    return gn;
}

/* 
This function will create and return a graph_node_list, creates a start node that is empty, initially start and end global variables both point to this
node.  Use start and end to check if they are equal to each other.  If start has a null name field, then the start node is empty and add to it.
*/
graph_node_list* create_graph_node_list(){
    graph_node_list* new = malloc(sizeof(graph_node_list));
    new->name = NULL;
    new->next = NULL;
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
        return;
        /*
        Don't add a next to it, we now have start and end pointing to the beginning node, once we 
        add nodes from here on out, we will create a new graph_node_list node and make ends next equal to it;
        set end->next to NULL so we make sure there are no junk values that throw us off
        */
    }
    /*
    Assume now that we do not have an empty list, therefore we use this logic:
    1. create new graph_node_list pointer
    2. have end->next point to it and move end to end->next
    3. set name of new graph_node_list to gn name, set addr = gn, set next = NULL
    4. increment size of gnl

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
   gnl_size++;
}
/* 
This function returns true if the passed in name exists as a graph node in the gnl.
This function iterates the list and assumes it starts at the beginning
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

    //curr_gnle :: curr_(g)raph_(n)ode_(l)ist_(e)lement, use this to iterate the list so that gnl is always pointing to the beginning of the list.
    graph_node_list* curr_gnle = gnl;
    while(curr_gnle != NULL && curr_gnle->addr->gnt != NULL){
        //isolate the target we are looking at
        target* curr_target = curr_gnle->addr->gnt;

        //isolate the list of dependencies for that target
        list_node* curr_dep = curr_target->dependencies;
        
        

        while(curr_dep != NULL){
            //if the current dependency we are examining does not exist in the gnl, create a new graph node and add it
            
            if(!exists_in_graph_node_list(gnl,curr_dep->val)){
                add_graph_node(create_non_target_graph_node(curr_dep));
            }
            curr_dep = curr_dep->next;
        }

        curr_gnle = curr_gnle->next;
    }
    build_dependency_graph(gnl);
    if(!check_no_cycle(gnl)){
        //fprintf(stderr,"Cycle detected!");
        return NULL;
    }
    return gnl;
}

/*
This function will return a pointer to graph_node pointers with size = to the size passed in.
If the size is 0, ie target has no dependencies, children ptr is set to NULL and returned
Use this to allocate space for graph_node children when building dependency graph.  DOES NOT point graph_node pointers to anything.
*/
graph_node** alloc_graph_node_children(int size){
    graph_node** children_ptr;
    if(size == 0){
        children_ptr = NULL;
    }else{
        children_ptr = (graph_node**)malloc(size*sizeof(graph_node*));
    }
    
    return children_ptr;
}
/*
ASSUMES YOU ARE PASSING IN FIRST ELEMENT OF GRAPH_NODE_LIST

This function will return the address of a graph_node given a name and a graph node list, if you built the graph_node_list from the info of the targets, a dependency should always exist
in the list.  If it doesn't this will return NULL.  Using build_gnl and using the same gnl in this function should make sure you never get a NULL return.
This function logic is similar/same as exists function
*/
graph_node* get_graph_node(graph_node_list* gnl, char* name){
    graph_node_list* curr = gnl;
    while(curr != NULL){
        if(strcmp(curr->name, name) == 0){
            //found a match, let's move on
            return curr->addr;
        }else{
            curr = curr->next;
        }
    }
    return NULL;
}

/*
This function will build a dependency graph given a built graph_node_list.  It will edit the graph_nodes within the graph_node_list.  The list can then be used to get a node where any
node will have it's dependency structure needed to build.  The algorithm works as follows:

    For each graph_node in the graph_node_list
        if it is a target graph_node
            allocate space for graph_node children w/ target->deps_size    
            for each dependency
                get address of dependency graph_node from list
                set target graph_node child pointer to it
*/
void build_dependency_graph(graph_node_list* gnl){
    graph_node_list* curr_gnle = gnl;
    while(curr_gnle != NULL){
        //isolate current graph_node
        graph_node* curr_graph_node = curr_gnle->addr;
        //isolate target of current graph_node
        target* curr_target = curr_graph_node->gnt;
        if(curr_target != NULL){
            curr_graph_node->children = alloc_graph_node_children(curr_target->deps_size);
            //isolate the current dependencies of the current target
            list_node* curr_dep = curr_target->dependencies;
            //use i to point children pointers
            int i = 0;
            while(curr_dep != NULL){
                curr_graph_node->children[i] = get_graph_node(gnl,curr_dep->val);
                curr_dep = curr_dep->next;
                i++;
            }
        }
        curr_gnle = curr_gnle->next;
    }
}

int check_no_cycle(graph_node_list* gnl){
    graph_node_list *ptr_gnl = gnl; 
    while(ptr_gnl){
        check_list = calloc(1,sizeof(graph_node_list));
        check_list->name = NULL;
        check_list->next = NULL;
        check = 0;
        if(!is_DAG(ptr_gnl->addr,ptr_gnl->name)){
            return 0;
        }
        free(check_list);
        ptr_gnl = ptr_gnl->next;
    }
    return 1;
}


int is_DAG(graph_node* gnode,char* name){
    if(strcmp(gnode->name,name)==0){
        if(!check){
            check = 1;
        }
        else{
            fprintf(stderr,"The element %s is duplicated!!\n",gnode->name);
            return 0;
        }
    }
    if(!exists_in_graph_node_list(check_list->next,gnode->name)){
        //add to the check list
        graph_node_list* new_gnl_node = malloc(sizeof(graph_node_list));
        new_gnl_node->name = gnode->name;
        new_gnl_node->addr = gnode;
        new_gnl_node->next = check_list->next;
        check_list->next = new_gnl_node;
    }
    
    //recursive on each children
    if(gnode->gnt && gnode->gnt->deps_size){
        for(int i = 0; i < gnode->gnt->deps_size;i++){
            if(!is_DAG(gnode->children[i],name)){
                return 0;
            } 
        }
    }

    return 1;
}