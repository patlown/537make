#include <stdio.h>

#include "proc_creation_prog_exe.h"
 
int main(int argc, char** argv){
	char *filename = NULL;
	char *target = NULL;
	int checkcall = 1;
	
	if(argc == 2){
		if(strcmp(argv[1],"-f")==0){
			checkcall = 0;
		}else{
			target = argv[1];
		}
	}else if(argc == 3){
		if(strcmp(argv[1],"-f")==0 && strcmp(argv[2],"-f")!=0){
			filename = argv[2];
		}else{
			checkcall = 0;
		}
	}else if(argc == 4){
		if(strcmp(argv[1],"-f")==0 && strcmp(argv[2],"-f")!=0 && strcmp(argv[3],"-f")!=0){
			filename = argv[2];
			target = argv[3];
		}else if(strcmp(argv[1],"-f")!=0 && strcmp(argv[2],"-f")==0 && strcmp(argv[3],"-f")!=0){
			filename = argv[3];
			target = argv[1];
		}else{
			checkcall = 0;
		}
	}else if(argc!=1){
		checkcall = 0;
	}

	if(!checkcall){
		fprintf(stderr,"Error: Incorrectly calling 537make!!\nusage: 537make [-f filename] [target]\n");
		exit(1);
	}

	char make_file[] = "makefile";
    if(filename == NULL){
		filename = make_file;
	}
	
	target_node *t = parseFile(filename);
	if(!t){
		exit(1);
	}
    // target_node *ptrt = t;
	// while(ptrt!=NULL){
	// 	printt(ptrt->t);
	// 	ptrt = ptrt->next;

	// }
	graph_node_list *list = build_graph_node_list(t);
	if(!list){
		exit(1);
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
	
	if(target==NULL){
		target = list->name;
	}
	makeTargets(target,list);
	return 0;
	// if(argc>4){
	// 	fprintf(stderr,"Error: Incorrectly calling 537make!!\nusage: 537make [-f filename] [target]\n");
	// 	exit(0);
	// }
	// if(argc == 1){
	// 	makeTargets(list->name,list);
	// }else if(argc == 2){
	// 	makeTargets(argv[1],list);
	// }
	// else if(argc == 3){
		
	// 	if(strcmp(argv[2],"-f")==0){

	// 	}else if(strcmp(argv[3],"-f")==0){

	// 	}
	// }
	// else{
	// 	fprintf(stderr,"Error: Incorrectly calling 537ps!!\n");
	// }

}
