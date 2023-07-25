#include "utilities.h"

int _strlen(const char *s)
{
    int i = 0;

    while (s[i] != '\0')
        i++;
    return i;
}

char *str_concat(const char *s1, const char *s2)
{
    char *new_str;
    int len1 = 0, len2 = 0, i = 0;

    if (s1)
        while (s1[len1])
            len1++;
    if (s2)
        while (s2[len2])
            len2++;

    new_str = malloc((len1 + len2 + 1) * sizeof(char));
    if (!new_str)
        return NULL;

    i = 0;
    if (s1)
        while (s1[i])
        {
            new_str[i] = s1[i];
            i++;
        }

    if (s2)
        while (*s2)
        {
            new_str[i] = *s2;
            i++, s2++;
        }

    new_str[i] = '\0';

    return new_str;
}

int _strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1 += 1;
        s2 += 1;
    }

    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

char *get_command_path(char *command)
{
    /* Actual implementation of get_command_path should go here. */
    /* This is just a placeholder. */
    char *path = malloc(100);
    strcpy(path, "/bin/");
    strcat(path, command);
    return path;
}

char *custom_getline(void)
{
    /* Actual implementation of custom_getline should go here. */
    /* This is just a placeholder. */
    char *line = malloc(100);
    fgets(line, 100, stdin);
    return line;
}

char **split_input(char *line)
{
    /* Actual implementation of split_input should go here. */
    /* This is just a placeholder. */
    char **args = malloc(64 * sizeof(char *));
    char *token;
    int position = 0;

    token = strtok(line, " \t\r\n\a");
    while (token != NULL) {
        args[position] = token;
        position++;
        token = strtok(NULL, " \t\r\n\a");
    }
    args[position] = NULL;
    return args;
}
