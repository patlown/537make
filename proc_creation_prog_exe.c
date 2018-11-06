#include "proc_creation_prog_exe.h"

/* This function checks if the node passed in exists as a target, prints an error to stderr if it does not 
and returns null.  Once the node is found, it calls the run commands function which runs commands bottom-up from the subgraph of
current node
*/
void makeTargets(char* name, graph_node_list* gnl){
    graph_node* curr_graph_node = NULL;
    graph_node_list* curr_gnle = gnl;

    //find the addr of the node we need to build
    while(curr_gnle != NULL){
        //check for matching name
        if(strcmp(curr_gnle->name,name) == 0){
            //check that it is a target, ie, the matching node contains a target node
            if(curr_gnle->addr->gnt != NULL){
                curr_graph_node = curr_gnle->addr;
                //once we've met these conditions, we break
                break;
            }  
        }
        curr_gnle = curr_gnle->next;
    }
    //if curr_graph_node stays null, then we never found the target in our graph_node_list
    if(curr_graph_node == NULL){
        fprintf(stderr,"%s is not a valid target in this makefile",name);
        exit(1);
    }
    //execute all commands of target recursively bottom up
    dfs(curr_graph_node);
}


/*
This function is given a graph node and recursively executes all its commands.  If target in graph node doesn't exist or is older than its dependency,
it is recompiled.  If target does exist and is not older than its dependencies, it is not recompiled
*/
void dfs(graph_node* node){
    //check if nodes have children, for each child, recurse
    if(node->gnt != NULL && node->gnt->deps_size > 0){
        for(int i = 0; i < node->gnt->deps_size; i++){
            dfs(node->children[i]);
        }
        //check for existence of file
        time_t parent_time = check_file_exists(node->gnt->name);
        printf("time of this file: %ld\n",parent_time);
        if(parent_time != 0){
            for(int i = 0; i< node->gnt->deps_size; i++){
                time_t child_time = check_file_exists(node->children[i]->name);
                printf("time of this file: %ld\n",child_time);
                if(child_time != 0){
                    if(child_time > parent_time){
                        runCommands(node);
                        break;
                    }
                }
            }
        }else{
        runCommands(node);
        }
    }else{
        runCommands(node);
    }

}


/*
This function handles fork and exec functions to run commands.  It expects to get correctly formatted program arguments
will print to stderr if a fork failed or executing commands failed.
This function takes inspiration from http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html
*/
void execute_curr(char* arg_line,char **argv){
    pid_t pid;
    int status;

    pid = fork();
    printf("%d\n",pid);
    if(pid < 0){
        fprintf(stderr,"Fork failed while processing line: %s\n",arg_line);
        exit(1);
    }
    if(pid == 0){
        execvp(*argv,argv);
        fprintf(stderr,"Failed to excute command: %s\n",arg_line);
        exit(1);
    }
    pid_t wait_result;

    while ((wait_result = wait(&status)) != -1)
    {
	if(status == 256){
		fprintf(stderr,"Failed to execute command: %s\n",arg_line);
		exit(1);
	}
    }
    return;
}
/*
This higher level function is given a graph node, checks if it contains a target, and if it does, runs the all the commands of the target
by calling parseCmds and execute_curr functions.
Does nothing if graph node isn't a target
*/
void runCommands(graph_node* node){
    if(node->gnt == NULL){
        return; 
    }
    list_node* curr_cmd = node->gnt->cmds;
    char* argv[arg_limit];
    for(int i = 0; i< node->gnt->cmds_size; i++){
        parseCmds(curr_cmd->val,argv);
        execute_curr(curr_cmd->val,argv);
        curr_cmd = curr_cmd->next;
    }
    return;
}
/*
This functions takes in a valid command line and parses it into
an argument array that can be passed in as command line arguments to a function
ex:
"gcc -o <filename>" -> {"gcc","-o","<filename>",NULL}
As long as argv size = buffer size, this functions correctly
This function takes inspiration from http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html
*/
void parseCmds(char *line, char** argv){
    char* ptr = line;
    int endofline = 0;
    int index = 0;
    while(!endofline)
    {
        while(*ptr == ' ' || *ptr == '\t' || *ptr == '\n'){
            if(*ptr == '\n'){
              *ptr = '\0';
              endofline = 1;
              break;  
            }
            *ptr++ = '\0';
        }
        argv[index] = ptr;
        
        if(*ptr != '\0'){
            index++;
        }
        while(*ptr != '\0' && *ptr != ' ' && *ptr != '\n' && *ptr != '\t'){
            ptr++;
        }
        
    }
    argv[index] = NULL;
}
/*
This function is given a filename in the current directory and appends a "./" to it.
The resulting return is "./<filename>"
*/
char* make_file_path(char* filename){
    char * app = "./";
    char * path;
    if((path = malloc(strlen(app)+strlen(filename)+1)) != NULL){
        path[0] = '\0';   // strcat operates on '\0' so make sure that is first letter of empty string
        strcat(path,app);
        strcat(path,filename);
    } else {
        fprintf(stderr,"Malloc failed to allocate space to store filepath './%s'",filename);
        exit(1);
    }
    return path;
}

/*returns last modified time if file does exist, else returns 0*/
time_t check_file_exists(char *filename){
    DIR *d;
    struct dirent *cd;
    //opens the  directory
    d = opendir("./");
    if (d != NULL)
    {
        //checks for file in directory
        while ((cd = readdir(d))){
            if(strcmp(cd->d_name,filename) == 0){
                //found the file we are looking for
                struct stat attr;
                char * path = make_file_path(filename);
                stat(path,&attr);
                //we are done with path, free it
                free(path);
                closedir(d);
                return attr.st_mtime;
            }
        }
        closedir(d);
    }
    else{
        fprintf (stderr,"Couldn't open the current directory");
        exit(1);
    }
    return 0;
}


// int main(){
//     // char line[1024] = "gcc -c -Wall -Wextra readproc.c\n";
//     // char  *argv[64]; 
//     // parseCmds(line,argv);
    
//     // int i = 0;
//     // while(argv[i] != NULL){
//     //     printf("%s\n",argv[i++]);
//     // }
//     // execute_curr(argv);

//     // char* filename = "test.txt";
//     // //char* filename2 = "readproc.c";
//     // time_t x = check_file_exists(filename);
//     // printf("%d\n", (int)x);

//     char file[15] = "text_parsing.o";
//     char line[1024] = "	make clean\n";
//     // char  *argv[64]; 
//     // parseCmds(line,argv);
//     // int i = 0;
//     // while(argv[i] != NULL){
//     //     printf("%s\n",argv[i++]);
//     // }

//     target* test = malloc(sizeof(target));
//     list_node* testcmds = malloc(sizeof(list_node));
//     testcmds->val = line;
//     testcmds->next = NULL;
//     test->cmds = testcmds;
//     test->cmds_size = 1;
//     test->deps_size = 0;
//     test->dependencies = NULL;
//     test->name = file;
    
//     graph_node* gn = malloc(sizeof(graph_node));
//     gn->gnt = test;
//     gn->name = test->name;
//     gn->children = NULL;

    
//     runCommands(gn);
// }
