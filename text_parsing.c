#include "text_parsing.h"

char* double_buff(char* buff, int cursize);
void skip_empty(int* c,FILE* fp);
char* read_line(int* c, FILE* fp);
target* get_target(int *c, FILE* fp);
int valid_cmd(char* s);
int valid_target(char* s);
int line_count;
int *c;
extern const int buffer_limit;
extern const int buffer_init;
int line = 0;

void printt(target *t);

/*TODO:
make sure target_node->next is pointing to NULL if it isn't pointing to another target node.
I rely on that when traversing the target_nodes while building the graph.

Also make sure the ends of the dependency and cmds list_nodes have a NULL next, as I'm assuming that when I traverse them.
*/

//TODO: can you briefly comment all the loops so we can see things at a higher level when reviewing the code.


target_node* parseFile(char * filename){
	FILE *fp;
	c = malloc(sizeof(int));
	target_node *dummy = malloc(sizeof(target_node));
	
	target_node *node_ptr = dummy;

	fp = fopen(filename, "r");
	if(fp == NULL){
		fprintf(stderr,"Cannot open file %s/n",filename);
		return NULL;
	}
	*c = fgetc(fp);



	
	
	while(1){
		skip_empty(c,fp);
		if(*c == EOF){
			break;
		}
		target *tempt=get_target(c,fp);
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


target* get_target(int *c, FILE* fp){
	target* t = malloc(sizeof(target));
	t->dependencies = malloc(sizeof(list_node));
	t->cmds = malloc(sizeof(list_node));
	list_node *nodeptr = t->dependencies;
	list_node *cmdptr = t->cmds;
	
	char* line = read_line(c,fp);
	char *s = line, *p = line;
	if(!valid_target(s)){
		//invalid target
		fprintf(stderr, "Invalid Target!\n Line:%d %s");

		return NULL;
	}	

	while(*p != '\n' && *p != ':'){
		p++;

	}
	if(*p == '\n'){
		//return incorrectly formatted line no :
		fprintf(stderr, "incorrectly formatted line no :\n");
		return NULL;
	}
	
	*p = '\0';
	t->name = s;
	//printf("%c\n",*p);
	
	int endofline = 0;
	while(!endofline){
		if(*p == '\n') break;
		p++;
		while(*p == ' '){p++;}
		s = p;
		while(*p!=' ' && *p!='\n'){ p++;}
		if(*p ==' '){
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

	}

	while(*c =='\t'){
		line = read_line(c,fp);
		if(line == NULL){
			break;
		}
		s = line;
		if(!valid_cmd(s)){
			fprintf(stderr, "Invalid Cmd!\n");
			return NULL;
		}
		list_node *tempnode = malloc(sizeof(list_node));
		tempnode->val = s;
		cmdptr->next = tempnode;
		cmdptr = cmdptr->next;

	}
	nodeptr->next = NULL;
	cmdptr->next = NULL;
	t->dependencies = t->dependencies->next;
	t->cmds = t->cmds->next;
	//num_targets++;
	return t;
}

int valid_target(char *s){
	if(*s == '\t' || *s == ' '){
		return 0;
	}
	return 1;
}
int valid_cmd(char *s){
	if(*s == '\t' && *(s+1)!='\t' && *(s+1)!='\n' && *(s+1)!=' '){
		return 1;
	}
	return 0;
}

/*




*/
void skip_empty(int* c,FILE* fp){
	while(*c == '\n' && *c != EOF){
		*c = fgetc(fp);
		line++;
	}
}

//assume not \n
char* read_line(int* c, FILE* fp){
	char* buff = malloc(buffer_init);
	int index = 0,cur_size = buffer_init;

	if(buff == NULL){
    		fprintf(stderr, "Memory Allocation failure of Buffer\n");
        	return NULL;
    }
	while(*c!='\n' && *c!=EOF ){
		if(index == cur_size){
			char* buff_new = double_buff(buff,cur_size);
			if(!buff_new){
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
	//check that line isn't empty
	if(index == 0){
		return NULL;
	}
	//

	if(index == cur_size){
			char* buff_new = double_buff(buff,cur_size);
			if(!buff_new){
				return NULL;
			}else{
				free(buff);
				buff = buff_new;
				cur_size*=2;
			}
	}
	buff[index] = '\n';
	line++;
	*c = fgetc(fp);
	return buff;
}

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
	return buff_double;
}

void printt(target* t){
	

		target *tar = t;
		printf("%s\n",tar->name);
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

}



int main(int argc, char** argv){
	target_node *t = parseFile(argv[1]);
    target_node *ptr = t;
	while(ptr!=NULL){
		printt(ptr->t);
		ptr = ptr->next;
	}
	// FILE *f = fopen(argv[1],"read");
	// int *c = malloc(sizeof(int));
	// *c = fgetc(f);
	// printf("%s",read_line(c,f));
	// skip_empty(c,f);
	// target *tar = get_target(c,f);
	// printt(tar);
	// printf("%s\n",tar->name);
	// list_node *ptr = tar->dependencies;

	// while(ptr!=NULL){
	// 	printf("%s ",ptr->val);
	// 	ptr = ptr->next;
	// }
	// list_node *ptr1 = tar->cmds;
	// while(ptr1!=NULL){
	// 	printf("%s ",ptr1->val);
	// 	ptr1 = ptr1->next;
	// }
	
	//target_node *root =parseFile(argv[1]);
	//print(root);
	
}

