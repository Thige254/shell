#ifndef SHELL_H
#define SHELL_H

/* Required standard libraries */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Function prototypes */
void interactive_shell(void);
char *custom_getline(void);
char **split_input(char *line);
int execute_command(char **args);

#endif /* SHELL_H */
