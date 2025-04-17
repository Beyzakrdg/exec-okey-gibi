#include "minishell.h"

static int ft_is_valid_unset(char *str)
{
    int i = 0;

    if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
    {
        printf("Not a valid identifier: %s\n", str);
        return 1;
    }

    while (str[i])
    {
        if (str[i] == '=')
            break;
        if (!(ft_isalnum(str[i]) || str[i] == '_'))
        {
            printf("Not a valid identifier: %s\n", str);
            return 1;
        }
        i++;
    }
    return 0;
}

void ft_unset_variable(t_program *prgm, char *variable)
{
    t_environment *prev = NULL;
    t_environment *current = prgm->exp_list;

    // exp_list'ten sil
    while (current)
    {
        if (ft_strcmp(current->variable, variable) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                prgm->exp_list = current->next;
            free(current->variable);
            free(current->value);
            free(current);
            break;
        }
        prev = current;
        current = current->next;
    }

    // env_list'ten sil
    prev = NULL;
    current = prgm->env_list;
    while (current)
    {
        if (ft_strcmp(current->variable, variable) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                prgm->env_list = current->next;
            free(current->variable);
            free(current->value);
            free(current);
            break;
        }
        prev = current;
        current = current->next;
    }
}

void ft_unset(t_program *prgm, char **args)
{
    int i = 1;

    while (args[i])
    {
        if (ft_is_valid_unset(args[i]) == 0)
        {
            char *cleaned = ft_quotes_clean(args[i]);
            ft_unset_variable(prgm, cleaned);
            free(cleaned);
        }
        i++;
    }
}
