#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 64
#define ARG_DELIMITER " \t\r\n\a"

char *custom_getline(void);
char **split_input(char *line);
char *get_command_path(char *command);
int execute_command(char **args);
void interactive_shell(void);

#endif /* _SHELL_H_ */
