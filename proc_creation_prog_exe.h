/*
Authors: Patrick Lown and Ge Xu
*/
#ifndef PROC_CREATION_PROG_EXE_H
#define PROC_CREATION_PROG_EXE_H

#include "text_parsing.h"
#include "build_spec_graph.h"
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>

//arg_limit is used when parsing command line arguments, this should not be larger than our buffer
static int const arg_limit = 1024;

/* This function checks if the node passed in exists as a target, prints an error to stderr if it does not 
and returns null.  Once the node is found, it calls the run commands function which runs commands bottom-up from the subgraph of
current node
*/
void makeTargets(char* name, graph_node_list* gnl);

/*
This function handles fork and exec functions to run commands.  It expects to get correctly formatted program arguments
will print to stderr if a fork failed or executing commands failed.
This function takes inspiration from http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html
*/
void execute_curr(char* arg_line,char **argv);

/*
This function is given a graph node and recursively executes all its commands.  If target in graph node doesn't exist or is older than its dependency,
it is recompiled.  If target does exist and is not older than its dependencies, it is not recompiled
*/
void dfs(graph_node* node);

/*
This functions takes in a valid command line and parses it into
an argument array that can be passed in as command line arguments to a function
ex:
"gcc -o <filename>" -> {"gcc","-o","<filename>",NULL}
As long as argv size = buffer size, this functions correctly
This function takes inspiration from http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html
*/
void parseCmds(char *line, char** argv);

/*
This function is given a filename in the current directory and appends a "./" to it.
The resulting return is "./<filename>"
*/
char* make_file_path(char* filename);

/*returns last modified time if file does exist, else returns 0*/
time_t check_file_exists(char *filename);

/*
This higher level function is given a graph node, checks if it contains a target, and if it does, runs the all the commands of the target
by calling parseCmds and execute_curr functions.
Does nothing if graph node isn't a target
*/
void runCommands(graph_node* node);


#endif