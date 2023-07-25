#include "shell.h"
#include "utilities.h"

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
            tokens = _realloc(tokens, position * sizeof(char*), bufsize * sizeof(char*));
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

char *get_command_path(char *command)
{
    char *path_var = _strdup(getenv("PATH"));
    char *path_dir = strtok(path_var, ":");
    char *full_path = NULL;

    while (path_dir != NULL)
    {
        full_path = str_concat(path_dir, "/");
        full_path = _realloc(full_path, strlen(full_path) + 1, strlen(full_path) + strlen(command) + 2);
        strcat(full_path, command);

        if (access(full_path, X_OK) == 0)
        {
            /* Command found and executable */
            free(path_var);
            return full_path;
        }

        free(full_path);
        path_dir = strtok(NULL, ":");
    }

    /* If command is not found in PATH, return command as is */
    free(path_var);
    return _strdup(command);
}

int execute_command(char *program_name, char **args)
{
    char *path;
    pid_t pid;
    int status;

    if (strcmp(args[0], "exit") == 0)
    {
        free(args);
        exit(EXIT_SUCCESS);
    }

    path = get_command_path(args[0]);
    if (access(path, X_OK) == -1)
    {
        print_error(program_name, args[0], ": command not found\n");
        free(path);
        return 1;
    }

    pid = fork();
    if (pid == 0)
    {
        /* Child process */
        if (execvp(path, args) == -1)
        {
            print_error(program_name, path, ": exec error\n");
            free(path);
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        /* Error forking */
        print_error(program_name, "fork", ": fork error\n");
        free(path);
        return 1;
    }
    else
    {
        /* Parent process */
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    free(path);
    return 1;
}

void interactive_shell(void)
{
    char *line;
    char **args;
    int status;

    do
    {
        print_prompt();
        line = custom_getline();
        args = split_input(line);
        if (args[0] != NULL)
            status = execute_command(args[0], args);

        free(line);
        free(args);
    } while (status);
}
