#include "text_parsing.h"
#include "build_spec_graph.h"
/*this function will build a target node list for adding children to target nodes, use this
once to get addresses to all the nodes.  When adding children to target nodes, check this list for addr
to specific node*/
target_node_list* build_target_node_list(target_node* tn){
    if(t == NULL){
        //target list is empty return null
        return NULL;
    }
    //build first node in list with first target
    target_node_list* first;
    target_node_list *tnl = malloc(sizeof(target_node_list));
    first = tnl;
    tnl->name = tn->t->name;
    tnl->addr = tn;
    

}