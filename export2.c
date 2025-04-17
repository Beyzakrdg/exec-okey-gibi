/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bekarada <bekarada@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:22:17 by bekarada          #+#    #+#             */
/*   Updated: 2025/04/14 15:22:17 by bekarada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

char *reconstruct_export_argument(t_token *start)
{
    size_t total_len = 0;
    t_token *temp = start;
    char *result;
    char *ptr;

    while (temp)
    {
        total_len += ft_strlen(temp->value);
        temp = temp->next;
    }
    result = malloc(total_len + 1);
    if (!result)
        return NULL;
    ptr = result;
    temp = start;
    while (temp)
    {
        size_t len = ft_strlen(temp->value);
        ft_memcpy(ptr, temp->value, len);
        ptr += len;
        temp = temp->next;
    }
    *ptr = '\0';
    return result;
}

int ft_is_valid_identifier(char *str)
{
    int i = 0;

    if (!str || (!(isalpha(str[0])) && str[0] != '_'))
    {
        printf("Not a valid identifier: %s. Variable name must start with a letter or underscore.\n", str);
        return 0;
    }
    
    while (str[i] && str[i] != '=')
    {
        if (!isalnum(str[i]) && str[i] != '_')
        {
            printf("Not a valid identifier: %s. Only letters, numbers, and underscores are allowed.\n", str);
            return 0;
        }
        i++;
    }
    
    if (str[i] == '=' && i == 0)
    {
        printf("Not a valid identifier: %s. Variable name cannot start with '='.\n", str);
        return 0;
    }

    return 1;
}

char *ft_quotes_clean(char *str)
{
    int i = 0, j = 0;
    char quote = 0;
    char *result = malloc(strlen(str) + 1);

    if (!str || !result)
        return NULL;

    while (str[i])
    {
        if ((str[i] == '\'' || str[i] == '\"') && quote == 0)
        {
            quote = str[i];
            i++;
            continue;
        }
        else if (quote != 0 && str[i] == quote)
        {
            quote = 0;
            i++;
            continue;
        }
        result[j++] = str[i++];
    }
    result[j] = '\0';
    return result;
}

void add_or_update_env(t_environment **list, char *variable, char *value)
{
    t_environment *exist = *list;
    t_environment *new_node;

    while (exist)
    {
        if (ft_strcmp(exist->variable, variable) == 0)
        {
            if (value && (ft_strcmp(exist->value, value) != 0))
            {
                free(exist->value);
                exist->value = ft_strdup(value);
            }
            return;
        }
        exist = exist->next;
    }

    new_node = (t_environment *)malloc(sizeof(t_environment));
    if (!new_node)
        return;

    new_node->variable = ft_strdup(variable);
    new_node->value = value ? ft_strdup(value) : "";
    new_node->next = *list;
    *list = new_node;
}

void ft_export_list(t_program *prgm, char *variable, char *value)
{
    add_or_update_env(&prgm->exp_list, variable, value);
    add_or_update_env(&prgm->env_list, variable, value);
    printf("Exported %s=%s\n", variable, value);
}

void free_tokens(char **tokens)
{
    if (!tokens)
        return;
    for (int i = 0; tokens[i]; i++)
        free(tokens[i]);
    free(tokens);
}
/*
char **ft_export_tokenize(char *export_str)
{
    char **tokens = malloc(sizeof(char *) * 3);
    char *variable;
    char *value = NULL;

    if (!tokens)
        return NULL;

    printf("Export string: '%s'\n", export_str);
    char *equal = ft_strchr(export_str, '=');
    if (equal)
    {
        printf("Found '=' at position: %ld\n", equal - export_str);
    }
    else
    {
        printf("No '=' found in the export string.\n");
    }

    if (equal)
    {
        variable = ft_strndup(export_str, equal - export_str);
        value = ft_strdup(equal + 1);
        if (value)
            value = ft_quotes_clean(value);
    }
    else
    {
        variable = ft_strdup(export_str);
        value = NULL;
    }

    if (!variable || (equal && !value && *equal != '\0'))
    {
        free(tokens);
        free(variable);
        if (value)
            free(value);
        return NULL;
    }

    tokens[0] = variable;
    tokens[1] = value;
    tokens[2] = NULL;

    return tokens;
}*/


int has_valid_quotes(const char *str)
{
    int single_quotes = 0;
    int double_quotes = 0;
    char last_quote = 0; // Son görülen tırnak türü

    while (*str)
    {
        if (*str == '\'')
        {
            single_quotes++;
            // Eğer son görülen tırnak türü aynı ise eşleşme var
            if (last_quote == '\'' && single_quotes % 2 == 0)
                return 0; // Aynı türde eşleşmeyen tırnaklar
            last_quote = '\''; // Tek tırnak son görülen tırnak türü
        }
        else if (*str == '\"')
        {
            double_quotes++;
            // Eğer son görülen tırnak türü aynı ise eşleşme var
            if (last_quote == '\"' && double_quotes % 2 == 0)
                return 0; // Aynı türde eşleşmeyen tırnaklar
            last_quote = '\"'; // Çift tırnak son görülen tırnak türü
        }
        str++;
    }
    return (single_quotes % 2 == 0 && double_quotes % 2 == 0);
}


char **ft_export_tokenize(char *export_str)
{
    char **tokens = malloc(sizeof(char *) * 3);
    char *variable;
    char *value = NULL;

    if (!tokens)
        return NULL;

    printf("Export string: '%s'\n", export_str);



    char *equal = ft_strchr(export_str, '=');
    if (equal)
    {
        printf("Found '=' at position: %ld\n", equal - export_str);
    }
    else
    {
        printf("No '=' found in the export string.\n");
    }

    if (equal)
    {
        variable = ft_strndup(export_str, equal - export_str);
        value = ft_strdup(equal + 1);

        // Eğer value tamamen çift tırnaklar içine alınmışsa ve içinde başka tırnak varsa geçersiz say
        if (value && ((value[0] == '"' && ft_strchr(value + 1, '"') != NULL) || (value[0] == '\'' && ft_strchr(value + 1, '\'') != NULL)))
        {
            char q = value[0];
            int len = ft_strlen(value);
            if (value[len - 1] == q && ft_strchr(value + 1, q) != value + len - 1)
            {
                printf("export: `%s`: not a valid assignment (nested quotes)\n", export_str);
                free(tokens);
                free(variable);
                free(value);
                return NULL;
            }
        }

        if (value)
            value = ft_quotes_clean(value);
    }
    else
    {
        variable = ft_strdup(export_str);
        value = NULL;
    }

    if (!variable || (equal && !value && *equal != '\0'))
    {
        free(tokens);
        free(variable);
        if (value)
            free(value);
        return NULL;
    }

    tokens[0] = variable;
    tokens[1] = value;
    tokens[2] = NULL;

    return tokens;
}


void ft_export(t_program *prgm, char *arg)
{
    char *variable = NULL;
    char *value = NULL;
    char **tokens;

    if (!arg || !arg[0])
        return;  // Empty argument case

    printf("Input to ft_export: '%s'\n", arg); 

    if(!has_valid_quotes(arg))
    {
        printf("%s not valid unmatchn",arg);
        return;
    }
    tokens = ft_export_tokenize(arg);
    if (!tokens)
        return;  // Tokenization failed, exit

    variable = tokens[0];
    value = tokens[1];

    if (!ft_is_valid_identifier(variable))
    {
        free_tokens(tokens);
        return;
    }

    ft_export_list(prgm, variable, value);

    free_tokens(tokens);
}
