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

void makeTargets(char* name, graph_node_list* gnl);
void execute_curr(char **argv);
void dfs(graph_node* node);
void parseCmds(char *line, char** argv);
char* make_file_path(char* filename);
time_t check_file_exists(char *filename);
void runCommands(graph_node* node);


#endif