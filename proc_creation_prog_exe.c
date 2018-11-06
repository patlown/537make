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
        return;
    }

    dfs(curr_graph_node);





}



void dfs(graph_node* node){
    //check if nodes have children, for each child, recurse
    if(node->gnt != NULL && node->gnt->deps_size > 0){
        for(int i = 0; i < node->gnt->deps_size; i++){
            dfs(node->children[i]);
        }
        //check for existence of file
        time_t parent_time = check_file_exists(node->gnt->name);
        if(parent_time != 0){
            for(int i = 0; i< node->gnt->deps_size; i++){
                time_t child_time = check_file_exists(node->children[i]->gnt->name);
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



void execute_curr(char **argv){
    pid_t pid;
    int status;

    // if((pid = fork())<0){
    //     fprintf(stderr,"failed to fork child process");

    //     return;
    // }
    // else if(pid == 0){
    //     printf("running as child");
    //     if(execvp(*argv,argv)<0){
    //         fprintf(stderr,"failed to execute the commands");
    //     }
    // }else{
    //     printf("waiting as parent");
    //     while(wait(&status) != pid){
    //         ;
    //     }
    // }

    pid = fork();
    printf("%d\n",pid);
    if(pid < 0){
        perror("fail fork\n");
        return;
    }
    if(pid == 0){
        execvp(*argv,argv);
        printf("failed to execute the commands");
        return;
    }
    pid_t wait_result;

    while ((wait_result = wait(&status)) != -1)
    {
        printf("Process %lu returned result: %d\n", (unsigned long) wait_result, status);
    }

    printf("Child has finished.\n");

    return;
}

void runCommands(graph_node* node){
    if(node->gnt == NULL){
        return; 
    }
    list_node* curr_cmd = node->gnt->cmds;
    char* argv[512];
    for(int i = 0; i< node->gnt->cmds_size; i++){
        parseCmds(curr_cmd->val,argv);
        execute_curr(argv);
        curr_cmd = curr_cmd->next;
    }
    return;
}

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
        //*argv = ptr;
        argv[index] = ptr;
        
        if(*ptr != '\0'){
            //*argv++;
            index++;
        }
        while(*ptr != '\0' && *ptr != ' ' && *ptr != '\n' && *ptr != '\t'){
            ptr++;
        }
        
    }
    //*argv = NULL;
    argv[index] = NULL;
}
char* make_file_path(char* filename){
    char * app = "./";
    char * path;
    if((path = malloc(strlen(app)+strlen(filename)+1)) != NULL){
        path[0] = '\0';   // strcat operates on '\0' so make sure that is first letter of empty string
        strcat(path,app);
        strcat(path,filename);
    } else {
        fprintf(stderr,"malloc failed!\n");
    }
    return path;
}

/*returns last modified time if file does exist, else returns 0*/
time_t check_file_exists(char *filename){
    DIR *d;
    struct dirent *cd;

    d = opendir("./");
    if (d != NULL)
    {
        while ((cd = readdir(d))){
            if(strcmp(cd->d_name,filename) == 0){
                //found the file we are looking for
                struct stat attr;
                char * path = make_file_path(filename);
                stat(path,&attr);
                closedir(d);
                return attr.st_mtime;
            }
        }
        closedir(d);
    }
    else{
        fprintf (stderr,"Couldn't open the directory");
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