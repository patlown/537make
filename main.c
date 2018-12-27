/*
Authors: Patrick Lown and Ge Xu
*/
#include <stdio.h>

#include "proc_creation_prog_exe.h"
 
int main(int argc, char** argv){
	char *filename = NULL;
	char *target = NULL;
	int checkcall = 1;
	//handle arguments
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
	
	/*
		Main logic of program, these three functions use our modules to get desired program result
	*/
	target_node *t = parseFile(filename);
	if(!t){
		exit(1);
	}
    
	graph_node_list *list = build_graph_node_list(t);
	if(!list){
		exit(1);
	}
	
	
	if(target==NULL){
		target = list->name;
	}
	makeTargets(target,list);
	return 0;
	
}
