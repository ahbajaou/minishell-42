#include "minishell.h"

char* custom(char* str, char* delims, char** saveptr) 
{
    char* token_start ;

    if (!str) 
        str = *saveptr;
    while (*str && strchr(delims, *str))
        str++;
    if (*str == '\0') 
    {
        *saveptr = str;
        return NULL;
    }
    token_start = str;
    while (*str && !strchr(delims, *str))
        str++;
    if (*str == '\0')
        *saveptr = str;
    else 
    {
        *str = '\0';
        *saveptr = str + 1;
    }
    return token_start;
}

char* custom_str(char* str, char* delims) 
{
    static char* next_token = NULL;
    char* token_start;

    if (!str) 
        str = next_token;
    while (*str && strchr(delims, *str)) 
        str++;
    if (*str == '\0') 
    {
        next_token = str;
        return NULL;
    }
    token_start = str;
    while (*str && !strchr(delims, *str)) 
        str++;
    if (*str) 
    {
        *str = '\0';
        next_token = str + 1;
    }
    else 
        next_token = str;
    return token_start;
}
