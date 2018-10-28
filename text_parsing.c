#include "text_parsing.h"
#include <stdio.h>
#include <string.h>


// target** parseFile(char * filename){
// 	FILE *fp;
// 	int endoffile;

// 	fp = fopen(filename, "r");
// 	if(fp == NULL){
// 		fprintf(stderr,"Cannot open file %s/n",filename);
// 		return NULL;
// 	}
// 	endoffile = 0;
// 	while(!endoffile){
// 		buff = malloc(buffer_init);
// 		char* c = fgetc(fp);
// 		get_next_target(c,fp) 


// 	}
// 	return NULL;
// }

// /*




// */
// char* get_next_target(char* c,FILE* fp){
// 	while(c == "\n" || c != EOF){
// 		c = fgetc(fp);
// 	}
// 	return NULL;
// }

char* create_target(char* line)
{
	const char* s = ":";
	char* tok;
	tok = strtok(line,s);

	while(tok != 0){
		printf("%s\n",tok);
		tok = strtok(0,s);
	}

	return line;
}

int main(){
	char x[] = "537ps:  537ps.o readproc.o parseopts.o output.o";
	const char* s = ":";
	char* tok;
	tok = strtok(x,s);

	while(tok != 0){
		printf("%s",tok);
		tok = strtok(0,s);
	}

}