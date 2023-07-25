#include "shell.h"

void interactive_shell(void)
{
    char *line;
    char **args;
    int status;

    do
    {
        write(STDOUT_FILENO, "Thige$ ", 7);
        line = custom_getline();
        args = split_input(line);
        if (args[0] != NULL)
            status = execute_command(args);
        free(line);
        free(args);
    } while (status);
}

char *custom_getline(void)
{
    char *line = NULL;
    size_t bufsize = 0;
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

    len = strlen(line);
    if (len > 0 && line[len - 1] == '\n')
    {
        line[len - 1] = '\0';
    }

    return line;
}

char **split_input(char *line)
{
    int bufsize = MAX_ARGS;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, ARG_DELIMITER);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;
        token = strtok(NULL, ARG_DELIMITER);
    }
    tokens[position] = NULL;

    return tokens;
}

int execute_command(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        /* Child process */
        if (execvp(args[0], args) == -1)
        {
            char error_message[200];
            sprintf(error_message, "Thige: %s: command not found\n", args[0]);
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        /* Error forking */
        perror("Error forking");
        return 1;
    }
    else
    {
        /* Parent process */
        do
        {
            waitpid(pid, &status, WUNTRACED);
        }
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}
