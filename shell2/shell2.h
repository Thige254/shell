#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ARG_DELIMITER " \t\r\n\a"
#define MAX_ARGS 64

void interactive_shell(void);
char *custom_getline(void);
char **split_input(char *line);
int execute_command(char **args);

#endif /* SHELL_H */
