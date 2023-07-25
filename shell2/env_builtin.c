#include "shell.h"
#include "utilities.h"
#include <unistd.h>

void print_prompt(void)
{
    write(STDOUT_FILENO, "Thige$ ", 7);
}

void print_error(char *program_name, char *error_text, char *message)
{
    int len_program = _strlen(program_name);
    int len_error = _strlen(error_text);
    int len_message = _strlen(message);

    write(STDERR_FILENO, program_name, len_program);
    write(STDERR_FILENO, ": ", 2);
    write(STDERR_FILENO, error_text, len_error);
    write(STDERR_FILENO, ": ", 2);
    write(STDERR_FILENO, message, len_message);
}

int execute_command(char *program_name, char **args)
{
    char *path;
    pid_t pid;
    int status;

    if (_strcmp(args[0], "exit") == 0)
    {
        exit(EXIT_SUCCESS);
    }

    if (_strcmp(args[0], "env") == 0)
    {
        for (int i = 0; environ[i] != NULL; i++)
        {
            write(STDOUT_FILENO, environ[i], _strlen(environ[i]));
            write(STDOUT_FILENO, "\n", 1);
        }
        return 1;
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
        if (execvp(path, args) == -1)
        {
            print_error(program_name, path, ": exec error\n");
            free(path);
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        print_error(program_name, "fork", ": fork error\n");
        free(path);
        return 1;
    }
    else
    {
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    free(path);
    return 1;
}

void interactive_shell(char *program_name)
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
            status = execute_command(program_name, args);

        free(line);
        free(args);
    } while (status);
}
