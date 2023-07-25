#include "shell.h"

/**
 * main - Entry point for the shell
 * @ac: Argument count
 * @av: Argument vector
 *
 * Return: Always 0 (Success)
 */
int main(__attribute__((unused))int ac, char **av)
{
    /* Start the shell */
    interactive_shell(av[0]);

    return (EXIT_SUCCESS);
}
