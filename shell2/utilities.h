#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int _strlen(const char *s);
int _strcmp(const char *s1, const char *s2);
char *_strcpy(char *dest, const char *src);
char *_strcat(char *dest, const char *src);
char *str_concat(const char *s1, const char *s2);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);

#endif /* UTILITIES_H */
