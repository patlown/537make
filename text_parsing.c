#include text_parsing.h



target** parseFile(char * filename){
	FILE *fp;

	fp = fopen(filename, "r");
	if(fp == NULL){
		fprintf(stderr,"Cannot open file %s/n",filename);
		return NULL;
	}
	endoffile = 0;
	while(!endoffile){
		buff = malloc(buffer_init);
		char* c = fgetc(fp);
		get_next_target(c,fp) 


}
/*




*/
char* get_next_target(char* c,FILE* fp){
	while(c == "\n" || c != EOF){
		c = fgetc(fp);
	}
}
int check_valid_target(char* c){


}