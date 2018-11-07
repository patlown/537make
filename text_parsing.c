#include "text_parsing.h"


int line_count;
int *c;  //ptr used to track the position we are currently reading




/* Note:
** target_node->next is pointing to NULL if it isn't pointing to another target node.
** Also the ends of the dependency and cmds list_nodes have a NULL next in the end of the list.
*/


/* This function is used to parse a make file giving file name
** it returns a ptr pointing to the first element of the target_node list
** return NULL if failed
*/
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
	dummy->next = NULL;
	//initialize the char buffer wich pointing to the position we are reading
	c = malloc(sizeof(int));
	*c = fgetc(fp);
	line_count = 0;
    
	//read throught the file 
	while(1){
		skip_empty(c,fp);
		//stop at EOF
		if(*c == EOF){
			break;
		}
		//we already skip empty lines and assuming this line as the begining of a target
		//try to get target
		target *tempt=get_target(c,fp);
		if(!tempt){   //get target failed, return NULL
			exit(1);
		}
		//create a target_node for this target and add the target_node list
		target_node *tempnode = malloc(sizeof(target_node));
		tempnode->t = tempt;
		tempnode->next = dummy->next;
		dummy->next = tempnode;
	}
	dummy = dummy->next;
	return dummy;
}

/* this func get a target assuming the fgetc points at the begining of a target return null if failed
*/
target* get_target(int *c, FILE* fp){
	//initialize the target and its dependency and cmds list node
	target* t = malloc(sizeof(target));
	t->dependencies = malloc(sizeof(list_node));
	t->cmds = malloc(sizeof(list_node));
	t->deps_size = 0;
	t->cmds_size = 0;
	list_node *nodeptr = t->dependencies;
	list_node *cmdptr = t->cmds;
	
	//read in the line which assumed to have target in it
	char* line = read_line(c,fp);
	if(!line) {
		free(t);
		return NULL;
	}
	line_count++;
	char *s = line, *p = line;
	
	//try to get the target name
	if(!valid_target(s)){
		//invalid target
		fprintf(stderr, "Invalid Target!\nLine No:%d\n%s",line_count,line);
		free(line);
		exit(1);
	}	
	while(*p != '\n' && *p != ':'){
		if(*p==' ' || *p=='\t'){
			//skip all empty spaces
			*p = '\0';
		}
		p++;
	}

	//ptr should be stopped at ':' otherwise the line is invalid
	if(*p == '\n'){
		//return incorrectly formatted line no :
		fprintf(stderr, "Incorrectly formatted line!\nLine No:%d\n%s",line_count,line);
		exit(1);
	}
	*p = '\0';
	t->name = s;//store the target name
	
	//try to get all the dependencies
	int endofline = 0;
	while(!endofline){
		p++;
		//skip all empty spaces
		while(*p == ' ' || *p == '\t'){p++;}
		if(*p == '\n') break;                         //if the line is ended just break
		s = p;                                        //when find a begining of a dependency add set the s pointing to that
		while(*p!=' ' && *p!='\n' && *p!='\t'){ p++;} //find the end of this dependency and set \0 to end that dependency  
		if(*p ==' ' || *p == '\t'){
			*p = '\0';
		}
		else{
			*p = '\0';
			endofline = 1; //if the dependency is terminated by new line, means the current line is processed 
		}
		
		//append to the end of dependency list
		list_node *tempnode = malloc(sizeof(list_node));
		tempnode->val = s;
		nodeptr->next = tempnode;
		nodeptr = nodeptr->next;
		t->deps_size++;
	}

	//store all the cmds
	while(*c =='\t' || *c == '\n'){
		//skip empty lines
		if(*c=='\n'){
			skip_empty(c,fp);
			continue;
		}
		//reading a cmd line and determine if its a calid one; if invalid return NULL
		line = read_line(c,fp);
		line_count++;
		if(!line) return NULL;
		s = line;
		if(!valid_cmd(s)){
			fprintf(stderr, "Invalid Cmd!\nLine No:%d\n%s",line_count,line);
			free(line);
			return NULL;
		}
		//add the new cmd line to the end of cmds list
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

/*func to determine if given line begin with valid target
*/
int valid_target(char *s){
	if(*s == '\t' || *s == ' '){
		return 0;
	}
	return 1;
}
/*function to determine if given line begin with valid cmd
*/
int valid_cmd(char *s){
	if(*s == '\t' && *(s+1)!='\t' && *(s+1)!='\n' && *(s+1)!=' '){
		return 1;
	}
	return 0;
}

/*this function skips empty lines and advances *c pointing to the first char after this line
*/
void skip_empty(int* c,FILE* fp){
	while(*c == '\n' && *c != EOF){
		*c = fgetc(fp);
		line_count++;
	}
}

/*this function readin a line, asuming not begin with '\n'
**return NULL if error occurs
*/
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
		free(buff);
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
			}
	}
	buff[index] = '\n';
	
	*c = fgetc(fp);
	return buff;
}

/* this function create a new buffer with doubled buffersize and copy its content
** return the newly created buff, and NULL if fail
*/
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

/*This is a test function for print name, dependencies and cmds also their size for a single target
*/
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

