#include "text_parsing.h"

char* double_buff(char* buff, int cursize);
void skip_empty(int* c,FILE* fp);
char* read_line(int* c, FILE* fp);

target_node* parseFile(char * filename){
	FILE *fp;

	fp = fopen(filename, "r");
	if(fp == NULL){
		fprintf(stderr,"Cannot open file %s/n",filename);
		return NULL;
	}
	// endoffile = 0;
	// while(!endoffile){
	// 	buff = malloc(buffer_init);
	int c = fgetc(fp);
	// 	get_next_target(c,fp) 
	return NULL;
}


target* get_target(int *c, FILE* fp){
	

	char* line = read_line(c,fp);
	char* s,p = line;
	if(!valid_target(s)){
		return NULL;
	}	

	while(*p != '\n' && *p != ':'){
		p++;

	}
	if(*p == '\n'){
		//return incorrectly formatted line no :
	}
	*p = '\0';
	t -> name = s;

	while(*p != '\n'){
		while( *p != '\n' && *p == ' ')
	}



}


/*




*/
void skip_empty(int* c,FILE* fp){
	while(*c == '\n' || *c != EOF){
		*c = fgetc(fp);
	}
}


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