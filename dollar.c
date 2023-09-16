#include "minishell.h"

size_t calculate_required_length(const char* input) 
{
    size_t total_length = 0;
    while (*input) 
    {
        if (*input == '$') 
        {
            input++;
            char var_name[256];
            char* var_start = var_name;
            while (*input && (isalnum(*input) || *input == '_')) 
                *var_start++ = *input++;
            *var_start = '\0';
            char* var_value = getenv(var_name);
            if (var_value)
                total_length += strlen(var_value);
        } 
        else 
        {
            total_length++;
            input++;
        }
    }
    return total_length;
}

int check_quotes(const char* input) 
{
    char quote_char = '\0';  // Utilisé pour suivre le type de guillemets que nous traitons
    while (*input) 
    {
        if (*input == '\'' && quote_char != '"') 
        {
            if (quote_char == '\'') 
                quote_char = '\0';
            else 
                quote_char = '\'';
            input++;
        } 
        else if (*input == '"' && quote_char != '\'') 
        {
            if (quote_char == '"') 
                quote_char = '\0';
            else 
                quote_char = '"';
            input++;
        } 
        else 
            input++;
    }
    return (quote_char == '\0');  // Retourne 1 si tous les guillemets sont correctement fermés, 0 sinon
}

char* read_input_with_quotes() 
{
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    char* full_input = malloc(1);
    full_input[0] = '\0';
    size_t full_len = 0;

    while ((read = getline(&line, &len, stdin)) != -1)
    {
        full_input = realloc(full_input, full_len + read + 1);
        strcat(full_input, line);
        full_len += read;

        if (full_input[full_len - 1] == '\n') 
        {
            full_input[full_len - 1] = '\0';  // Remove newline
            if (check_quotes(full_input)) 
                break;
            else 
                printf("> ");  // Prompt for continuation of input
        }
    }
    free(line);
    return full_input;
}

// char* replace_env_vars(const char* input) 
// {
//     size_t required_length = calculate_required_length(input);
//     char* result = malloc(required_length + 1);
//     if (!result) 
//     {
//         perror("malloc");
//         exit(1);
//     }
    
//     char* current = result;
//     char quote_char = '\0';
//     char prev_char = '\0';  // Ajout d'une variable pour suivre le caractère précédent

//     while (*input) 
//     {
//         if (*input == '\'' && quote_char != '"' && prev_char != '\\') 
//         {
//             if (quote_char == '\'') 
//                 quote_char = '\0';
//             else 
//                 quote_char = '\'';
//             input++;
//         } 
//         else if (*input == '"' && quote_char != '\'' && prev_char != '\\') 
//         {
//             if (quote_char == '"') 
//                 quote_char = '\0';
//             else 
//                 quote_char = '"';
//             input++;
//         } 
//         else if (*input == '$' && quote_char != '\'') 
//         {
//             input++;
//             char var_name[256];
//             char* var_start = var_name;
//             while (*input && (isalnum(*input) || *input == '_')) 
//                 *var_start++ = *input++;
//             *var_start = '\0';
//             char* var_value = getenv(var_name);
//             if (var_value) 
//             {
//                 strcpy(current, var_value);
//                 current += strlen(var_value);
//             }
//         } 
//         else 
//         {
//             if (*input == '\\' && (quote_char == '"' || quote_char == '\'') && (input[1] == quote_char)) 
//             {
//                 *current++ = input[1];
//                 input += 2;
//             } 
//             else 
//                 *current++ = *input++;
//         }
//         prev_char = *(input - 1);  // Mettre à jour le caractère précédent
//     }
//     *current = '\0';
//     return result;
// }
char* replace_env_vars(const char* input) 
{
    size_t required_length = calculate_required_length(input);
    char* result = malloc(required_length + 1);
    if (!result) {
        perror("malloc");
        exit(1);
    }

    char* current = result;
    char quote_char = '\0';
    int is_escaped = 0;  // Utilisé pour suivre si le caractère actuel est échappé

    while (*input) 
    {
        if (*input == '\\' && (input[1] == '"' || input[1] == '\'')) 
        {
            *current++ = input[1];
            input += 2;
            continue;
        }

        if (*input == '\'' && !is_escaped && quote_char != '"') 
        {
            if (quote_char == '\'') 
                quote_char = '\0';
            else 
                quote_char = '\'';
        } 
        else if (*input == '"' && !is_escaped && quote_char != '\'') 
        {
            if (quote_char == '"') 
                quote_char = '\0';
            else 
                quote_char = '"';
        } 
        else if (*input == '$' && quote_char != '\'') 
        {
            input++;
            char var_name[256];
            char* var_start = var_name;
            while (*input && (isalnum(*input) || *input == '_')) 
                *var_start++ = *input++;
            *var_start = '\0';
            char* var_value = getenv(var_name);
            if (var_value) 
            {
                strcpy(current, var_value);
                current += strlen(var_value);
            }
        } 
        else 
        {
            *current++ = *input;
        }

        if (*input == '\\') 
            is_escaped = !is_escaped;
        else 
            is_escaped = 0;

        input++;
    }
    *current = '\0';
    return result;
}