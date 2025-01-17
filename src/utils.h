#ifndef BEAM_UTILS_H
#define BEAM_UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

/** split a string into an array of strings
 *  internally this function uses strtok_r
 */

typedef struct split_r
{
    char **tokens;
    size_t count;
} split_r;

void free_split(split_r *split)
{
    if (split == NULL)
        return;

    if (split->tokens != NULL)
    {
        for (int i = 0; i < split->count; i++)
        {
            free(split->tokens[i]);
        }
        free(split->tokens);
    }
    free(split);
}

split_r *split(char *str, const char *delim, int max_tokens)
{
    if (max_tokens <= 0 || strlen(str) == 0 || delim == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    split_r *result = (split_r *)malloc(sizeof(split_r));

    if (result == NULL)
    {
        errno = ENOMEM;
        perror("could not allocate memory for split_r");
        return NULL;
    }

    char **tokens = (char **)malloc(sizeof(char *) * max_tokens);
    if (tokens == NULL)
    {
        errno = ENOMEM;
        perror("could not allocate memory for tokens");
        free(result);
        return NULL;
    }

    result->tokens = tokens;
    result->count = 0;

    char *dupe = strdup(str);
    if (dupe == NULL)
    {
        perror("could not duplicate string");
        free(result);
        free(tokens);
        return NULL;
    }

    char *save_ptr;
    char *token = strtok_r(dupe, delim, &save_ptr);

    while (token != NULL && result->count < max_tokens)
    {
        tokens[result->count] = strdup(token);
        if (tokens[result->count] == NULL)
        {
            free(dupe);
            free_split(result);
            return NULL;
        }
        result->count++;
        token = strtok_r(NULL, delim, &save_ptr);
    }

    free(dupe);
    return result;
}

char *find_and_terminate(char *str, char find)
{
    str = strchr(str, find);
    if (str == NULL)
    {
        return NULL;
    }
    *str = '\0';
    return str + 1;
}
#endif
