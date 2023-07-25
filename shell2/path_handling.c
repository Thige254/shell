#include "shell.h"

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
        print_error("split_input", "Allocation error\n", "");
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

char *get_command_path(char *command)
{
    char *path_var = _strdup(getenv("PATH"));
    char *path_dir = strtok(path_var, ":");
    char *full_path = NULL;

    while (path_dir != NULL)
    {
        full_path = str_concat(str_concat(path_dir, "/"), command);
        if (access(full_path, X_OK) == 0)
        {
            free(path_var);
            return full_path;
        }
        free(full_path);
        path_dir = strtok(NULL, ":");
    }
    free(path_var);
    return _strdup(command);
}

int execute_command(char *program_name, char **args)
{
    pid_t pid;
    int status;

    char *path = get_command_path(args[0]);

    if (access(path, X_OK) == -1)
    {
        print_error(program_name, args[0], ": command not found\n");
        free(path);
        return 1;
    }

    pid = fork();
    if (pid == 0)
    {
        if (execve(path, args, NULL) == -1)
        {
            perror(args[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        perror("Error forking");
    }
    else
    {
        do
        {
            pid = wait(&status);
        }
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    free(path);
    return 1;
}

void interactive_shell(char *program_name)
{
    char *line;
    char **args;

    do
    {
        print_prompt();
        line = custom_getline();
        args = split_input(line);
        if (args[0] != NULL)
        {
            execute_command(program_name, args);
        }
        free(line);
        free(args);
    }
    while (1);
}
