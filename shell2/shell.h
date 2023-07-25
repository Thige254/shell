#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "utilities.h"

#define MAX_ARGS 64
#define ARG_DELIMITER " \t\r\n\a"

extern char **environ;

void interactive_shell(char *program_name);
char *custom_getline(void);
char **split_input(char *line);
char *get_command_path(char *command);
int execute_command(char *program_name, char **args);
void print_prompt(void);
void print_error(char *program_name, char *command, char *message);

#endif /* SHELL_H */
