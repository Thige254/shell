#include "shell.h"

void interactive_shell(void)
{
    char *line;
    char **args;

    do
    {
        write(STDOUT_FILENO, "Thige$ ", 7);
        line = custom_getline();
        args = split_input(line);
        execute_command(args);

        free(line);
        free(args);
    } while (1);
}

char *custom_getline(void)
{
    char *line = NULL;
    size_t bufsize = 0; /* Let getline handle dynamic memory allocation */
    size_t len;

    if (getline(&line, &bufsize, stdin) == -1)
    {
        if (feof(stdin))
        {
            write(STDOUT_FILENO, "\n", 1);
            exit(EXIT_SUCCESS);
        }
        else
        {
            perror("custom_getline: getline error");
            exit(EXIT_FAILURE);
        }
    }

    /* Remove the newline character if it exists */
    len = strlen(line);
    if (len > 0 && line[len - 1] == '\n')
        line[len - 1] = '\0';

    return line;
}

char **split_input(char *line)
{
    int bufsize = MAX_ARGS, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "split_input: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, ARG_DELIMITER);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += MAX_ARGS;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens)
            {
                fprintf(stderr, "split_input: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, ARG_DELIMITER);
    }
    tokens[position] = NULL;
    return tokens;
}

int execute_command(char **args)
{
    pid_t pid;  /* Declaration moved up here */
    int status;

    if (args[0] == NULL)
    {
        /* An empty command was entered */
        return 1;
    }

    if (strcmp(args[0], "exit") == 0)
    {
        exit(EXIT_SUCCESS);
    }

    pid = fork();  /* Removed the pid_t from here */

    if (pid == 0)
    {
        /* Child process */
        if (execvp(args[0], args) == -1)
        {
            char error_message[30] = "Thige: ";
            strcat(error_message, args[0]);
            perror(error_message);
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        /* Error forking */
        perror("execute_command: fork error");
        return EXIT_FAILURE;
    }
    else
    {
        /* Parent process */
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return (status == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
