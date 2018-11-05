// #include "build_spec_graph.h"
// #include "text_parsing.h"

/* This function checks if the node passed in exists as a target, prints an error to stderr if it does not 
and returns null.  Once the node is found, it calls the run commands function which runs commands bottom-up from the subgraph of
current node
*/
// void makeTargets(char* name, graph_node_list* gnl){
//     graph_node* curr_graph_node = NULL;
//     graph_node_list* curr_gnle = gnl;

//     //find the addr of the node we need to build
//     while(curr_gnle != NULL){
//         //check for matching name
//         if(strcmp(curr_gnle->name,name) == 0){
//             //check that it is a target, ie, the matching node contains a target node
//             if(curr_gnle->addr->gnt != NULL){
//                 curr_graph_node = curr_gnle->addr;
//                 //once we've met these conditions, we break
//                 break;
//             }  
//         }
//     }
//     //if curr_graph_node stays null, then we never found the target in our graph_node_list
//     if(curr_graph_node == NULL){
//         fprintf(stderr,"%s is not a valid target in this makefile",name);
//         return NULL;
//     }






// }



void dfs(graph_node* node){
    //check if nodes have children, for each child, recurse
    if(node->gnt != NULL && node->gnt->deps_size > 0){
        for(int i = 0; i < deps_size; i++){
            dfs(node->children[i]);
        }
        //check for existence of file
        time_t parent_time = check_file_exists(node->gnt->name);
        if(parent_time != 0){
            for(int i = 0; i<deps_size; i++){
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


#include <stdio.h>
#include  <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
void execute_curr(char **argv){
    pid_t pid;
    int status;

    if((pid = fork())<0){
        fprintf(stderr,"failed to fork child process");
        return;
    }
    else if(pid == 0){
        printf("running as child");
        if(execvp(*argv,argv)<0){
            fprintf(stderr,"failed to execute the commands");
        }
    }else{
        printf("waiting as parent");
        while(wait(&status) != pid){
            continue;
        }
    }
}

// runCommands(graph_node* node){
    
//     for(int i = 0; i< node->gnt->cmds_size; i++){
//         pid_t pid = fork();
//         if(pid == -1){
//             //failed fork TODO
//         }else if(pid > 0){
//             int status;
//             waitpid(pid,&status,0);
//         }else{
//             execve()
//         }
//     }
// }

void parseCmds(char *line, char** argv){
    char* ptr = line;
    int endofline = 0;
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

        *argv = ptr;
        
        if(*ptr != '\0'){
            *argv++;
        }
        while(*ptr != '\0' && *ptr != ' ' && *ptr != '\n' && *ptr != '\t'){
            ptr++;
        }
        
    }
    *argv = NULL;
}
char* make_file_path(char* filename){
    char * app = "./";
    char * path;
    if((path = malloc(strlen(app)+strlen(filename)+1)) != NULL){
        path[0] = '\0';   // strcat operates on '\0' so make sure that is first letter of empty string
        strcat(path,app);
        strcat(path,filename);
    } else {
        fprintf(STDERR,"malloc failed!\n");
    }
}
#include <time.h>
#include <sys/stat.h>
/*returns last modified time if file does exist, else returns 0*/
time_t check_file_exists(char *filename){
    DIR *d;
    struct dirent *cd;

    d = opendir ("./");
    if (d != NULL)
    {
        while (cd = readdir (d)){
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

#include <dirent.h>
#include <string.h>
int main(){
    char line[1024] = "gcc -c -Wall -Wextra readproc.c\n";
    char  *argv[64]; 
    parseCmds(line,argv);
    
    int i = 0;
    while(argv[i] != NULL){
        printf("%s\n",argv[i++]);
    }
    execute_curr(argv);
    
    

}


