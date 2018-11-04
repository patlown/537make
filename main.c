#include <stdio.h>

#include "build_spec_graph.h"
#include <unistd.h>


int main(int argc, char** argv){
	target_node *t = parseFile(argv[1]);
    //target_node *ptrt = t;
	// while(ptrt!=NULL){
	// 	printt(ptrt->t);
	// 	ptrt = ptrt->next;

	// }
	graph_node_list *list = build_graph_node_list(t);
	
	// while(ptrlist!=NULL){
	// 	printf("%s\n",ptrlist->name);
	// 	ptrlist = ptrlist->next;
	// }
	// graph_node_list* curr_gnle = list;
	// while(curr_gnle != NULL){
	// 	if(curr_gnle->addr->children != NULL){
	// 		printf("This Node has children: %s\n",curr_gnle->name);
	// 		for(int i = 0; i < curr_gnle->addr->gnt->deps_size; i++){
	// 			printf("Child %d: %s\n",i,curr_gnle->addr->children[i]->name);
	// 		 }
	// 	}
	// 	curr_gnle = curr_gnle->next;
	// }

	char* cmd = "gcc main.c build_spec_graph.c";
	execvp(cmd);
	//check out this link: http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html
	return 0;
}
