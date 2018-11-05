#include "text_parsing.h"


int line_count;
int *c;
//extern const int buffer_limit;
//extern const int buffer_init;



/*TODO:
make sure target_node->next is pointing to NULL if it isn't pointing to another target node.
I rely on that when traversing the target_nodes while building the graph.

Also make sure the ends of the dependency and cmds list_nodes have a NULL next, as I'm assuming that when I traverse them.
*/

//TODO: can you briefly comment all the loops so we can see things at a higher level when reviewing the code.


target_node* parseFile(char * filename){
	//open file
	FILE *fp;
	fp = fopen(filename, "r");
	if(fp == NULL){
		fprintf(stderr,"Cannot open file %s\n",filename);
		return NULL;
	}
	//create the start of the traget list
	target_node *dummy = malloc(sizeof(target_node));
	target_node *node_ptr = dummy;
	//initialize the char buffer
	c = malloc(sizeof(int));
	*c = fgetc(fp);
	line_count = 0;


	
	
	while(1){
		skip_empty(c,fp);
		if(*c == EOF){
			break;
		}
		target *tempt=get_target(c,fp);
		if(!tempt) return NULL;
		//printt(tempt);
		if(!tempt){
			//print error 
			return NULL;
		}
		target_node *tempnode = malloc(sizeof(target_node));
		tempnode->t = tempt;
		node_ptr->next = tempnode;
		node_ptr = node_ptr->next;
	}
	node_ptr->next = NULL;
	return dummy->next;

	
}

//get a target assuming the fgetc points at the begining of a target return null if failed
target* get_target(int *c, FILE* fp){
	//initialize the target and its dependency and cmds list node
	target* t = malloc(sizeof(target));
	t->dependencies = malloc(sizeof(list_node));
	t->cmds = malloc(sizeof(list_node));
	t->deps_size = 0;
	t->cmds_size = 0;
	list_node *nodeptr = t->dependencies;
	list_node *cmdptr = t->cmds;
	
	char* line = read_line(c,fp);
	if(!line) return NULL;
	line_count++;
	char *s = line, *p = line;
	
	//try to get the target name
	if(!valid_target(s)){
		//invalid target
		fprintf(stderr, "Invalid Target!\nLine No:%d\n%s",line_count,line);

		return NULL;
	}	
	while(*p != '\n' && *p != ':'){
		if(*p==' ' || *p=='\t'){
			fprintf(stderr, "Invalid Target!\nLine No:%d\n%s",line_count,line);
			return NULL;
		}
		p++;
	}
	if(*p == '\n'){
		//return incorrectly formatted line no :
		fprintf(stderr, "Incorrectly formatted line!\nLine No:%d\n%s",line_count,line);
		return NULL;
	}
	*p = '\0';
	t->name = s;
	
	//try to get all the dependencies
	int endofline = 0;
	while(!endofline){
		
		p++;
		
		while(*p == ' ' || *p == '\t'){p++;}
		if(*p == '\n') break;
		s = p;
		while(*p!=' ' && *p!='\n' && *p!='\t'){ p++;}
		if(*p ==' ' || *p == '\t'){
			*p = '\0';
		}
		else{
			*p = '\0';
			endofline = 1;
			//printf("check\n");
		}
		
		list_node *tempnode = malloc(sizeof(list_node));
		//tempnode->next = NULL;
		tempnode->val = s;
		nodeptr->next = tempnode;
		nodeptr = nodeptr->next;
		t->deps_size++;

	}

	//store all the cmds
	while(*c =='\t'){
		line = read_line(c,fp);
		line_count++;
		if(!line)return NULL;
		s = line;
		if(!valid_cmd(s)){
			fprintf(stderr, "Invalid Cmd!\nLine No:%d\n%s",line_count,line);
			return NULL;
		}
		list_node *tempnode = malloc(sizeof(list_node));
		tempnode->val = s;
		cmdptr->next = tempnode;
		cmdptr = cmdptr->next;
		t->cmds_size++;
	}

	//end the deps and cmds list with NULL and move pointer to start of each list
	nodeptr->next = NULL;
	cmdptr->next = NULL;
	t->dependencies = t->dependencies->next;
	t->cmds = t->cmds->next;
	//num_targets++;
	return t;
}

//determine if given line begin with valid target
int valid_target(char *s){
	if(*s == '\t' || *s == ' '){
		return 0;
	}
	return 1;
}
//determine if given line begin with valid cmd
int valid_cmd(char *s){
	if(*s == '\t' && *(s+1)!='\t' && *(s+1)!='\n' && *(s+1)!=' '){
		return 1;
	}
	return 0;
}

//this function skip empty lines
void skip_empty(int* c,FILE* fp){
	while(*c == '\n' && *c != EOF){
		*c = fgetc(fp);
		line_count++;
	}
}

//function readin a line ssume not \n this 
char* read_line(int* c, FILE* fp){
	char* buff = malloc(buffer_init);
	int index = 0,cur_size = buffer_init;

	if(buff == NULL){
    		fprintf(stderr, "Memory Allocation failure of Buffer\n");
        	return NULL;
    }
	//readin to the buffer one char by one if buffersize exceed double the buffer
	while(*c!='\n' && *c!=EOF ){
		if(index == cur_size){
			char* buff_new = double_buff(buff,cur_size);
			if(!buff_new){
				fprintf(stderr,"Line No:%d\n%s\n",line_count,buff);
				return NULL;
			}else{
				free(buff);
				buff = buff_new;
				cur_size*=2;
			}
		}
		buff[index++] = (char)*c;
		*c = fgetc(fp);
	}
	//check that line isn't empty (this shouldn't happen)
	if(index == 0){
		return NULL;
	}
    //try to append an '\n' at end
	if(index == cur_size){
			char* buff_new = double_buff(buff,cur_size);
			if(!buff_new){
				fprintf(stderr,"Line No:%d\n%s\n",line_count,buff);
				return NULL;
			}else{
				free(buff);
				buff = buff_new;
				cur_size*=2;
			}
	}
	buff[index] = '\n';
	
	*c = fgetc(fp);
	return buff;
}

//this function create a new buffer with doubled buffersize and copy its content
char* double_buff(char* buff, int cursize){
	if(cursize == buffer_limit){
				fprintf(stderr, "Line exceed max line limit!!\n");
       			return NULL;
	}
			
	char* buff_double = malloc(cursize*2);
	if(buff_double == NULL){
    	fprintf(stderr, "Memory Allocation failure of Buffer\n");
        return NULL;
    }
	for(int i = 0; i < cursize; i++){
		buff_double[i] = buff[i];
	}
	//free(buff);
	return buff_double;
}

//test function for print name, dependencies and cmds for a single target
void printt(target* t){
		target *tar = t;
		printf("%s\n",tar->name);
		printf("%d\n",tar->deps_size);
		printf("%d\n",tar->cmds_size);
		list_node *ptr = tar->dependencies;
		int i = 1;
		while(ptr!=NULL){
			printf("Dependency %d: %s ",i,ptr->val);
			ptr = ptr->next;
			i++;
		}
		printf("\n");
		list_node *ptr1 = tar->cmds;
		while(ptr1!=NULL){
			printf("%s",ptr1->val);
			ptr1 = ptr1->next;
		}
		printf("------------------\n");

}



// int main(int argc, char** argv){
// 	target_node *t = parseFile(argv[1]);
//     target_node *ptr = t;
// 	while(ptr!=NULL){
// 		printt(ptr->t);
// 		ptr = ptr->next;
// 	}




// 	// FILE *f = fopen(argv[1],"read");
// 	// int *c = malloc(sizeof(int));
// 	// *c = fgetc(f);
// 	// printf("%s",read_line(c,f));
// 	// skip_empty(c,f);
// 	// target *tar = get_target(c,f);
// 	// printt(tar);
// 	// printf("%s\n",tar->name);
// 	// list_node *ptr = tar->dependencies;

// 	// while(ptr!=NULL){
// 	// 	printf("%s ",ptr->val);
// 	// 	ptr = ptr->next;
// 	// }
// 	// list_node *ptr1 = tar->cmds;
// 	// while(ptr1!=NULL){
// 	// 	printf("%s ",ptr1->val);
// 	// 	ptr1 = ptr1->next;
// 	// }
	
// 	//target_node *root =parseFile(argv[1]);
// 	//print(root);
	
// }

