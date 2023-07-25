#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_ARGS 64
#define ARG_DELIMITER " \t\r\n\a"

char *custom_getline(void);
char **split_input(char *line);
char *get_command_path(char *command);
int execute_command(char **args);
void interactive_shell(void);

#endif /* COMMON_H */
