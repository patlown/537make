#include <stdio.h>

#include "proc_creation_prog_exe.h"

int main(int argc, char** argv){
	
	char make_file[] = "test.txt";
	target_node *t = parseFile(make_file);
	if(!t){
		return -1;
	}else {
		printf("parser completed!!\n");
	}
    // target_node *ptrt = t;
	// while(ptrt!=NULL){
	// 	printt(ptrt->t);
	// 	ptrt = ptrt->next;

	// }
	graph_node_list *list = build_graph_node_list(t);
	if(!list){
		return -1;
	}else {
		printf("graph completed!!\n");
	}
	//graph_node_list *list_ptr = list;
	// while(list_ptr!=NULL){
	// 	printf("%s\n",list_ptr->name);
	// 	list_ptr = list_ptr->next;
	// }
	
	
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

	if(argc == 1){
		makeTargets(list->name,list);
	}else if(argc == 2){
		makeTargets(argv[1],list);
	}else{
		fprintf(stderr,"error\n");
	}

	return 0;
}
