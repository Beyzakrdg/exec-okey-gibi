/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bekarada <bekarada@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 14:43:04 by bekarada          #+#    #+#             */
/*   Updated: 2025/03/25 18:42:55 by bekarada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_valid_export(char *str)
{
	int	i;

	i = 0;
	if (!str || ((!ft_isalpha(str[0])) || str[0] == '_'))
	{
		printf("Not a valid identifier %s\n", str);
		return (0);
	}
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
		{
			printf("Not a valid identifier\n %s", str);
			return (0);
		}
		i++;
	}
	return (1);
}

// char	*ft_quotes_clean(char *str)
// {
// 	int		i;
// 	char	*without_quotes;

// 	i = 0;
// 	if (!str)
// 		return (NULL);
// 	if (str[0] == '\"' || str[0] == '\'')
// 	{
// 		if (str[0] == '\'')
// 		{	
// 			i++;
// 			while (str[i] != '\'' && str[i])
// 				i++;
// 		}
// 		else if (str[0] == '\"')
// 		{
// 			i++;
// 			while (str[i] != '\"' && str[i])
// 				i++;
// 		}
// 		without_quotes = ft_strndup(str + 1, i - 1);
// 	}
// 	else
// 		without_quotes = ft_strdup(str);
// 	return (without_quotes);
// }

char *ft_quotes_clean(char *str)
{
    int start = 0;
    char *cleaned_str;
    char quote_char;

    if (!str)
        return NULL;

    if (str[start] == '"' || str[start] == '\'') {
        quote_char = str[start];
        start++;
        while (str[start] && str[start] != quote_char) {
            start++;
        }
        if (str[start] == quote_char) {
            start++;
        }
    }
    cleaned_str = ft_strndup(str + start, ft_strlen(str) - start);
    if (!cleaned_str)
        return NULL;

    return cleaned_str;
}


void ft_export_list(t_program *prgm, char *variable, char *value)
{
    t_environment *exist;
    t_environment *new_node;

    exist = prgm->exp_list;
    while (exist)
    {
        if (strcmp(exist->variable, variable) == 0)
        {
            if (value)
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
    new_node->value = (value) ? ft_strdup(value) : NULL;
    new_node->next = prgm->exp_list;
    prgm->exp_list = new_node;

    exist = prgm->env_list;
    while (exist)
    {
        if (strcmp(exist->variable, variable) == 0)
        {
            free(exist->value);
            exist->value = ft_strdup(value);
            return;
        }
        exist = exist->next;
    }
    new_node = (t_environment *)malloc(sizeof(t_environment));
    if (!new_node)
        return;
    new_node->variable = ft_strdup(variable);
    new_node->value = ft_strdup(value);
    new_node->next = prgm->env_list;
    prgm->env_list = new_node;
}

char **ft_export_tokenize(char *export_str)
{
    char **tokens;
    char *variable;
    char *value;
    char *equal;
    int i = 0;

    tokens = malloc(sizeof(char *) * 3);
    if (!tokens)
        return NULL;
    export_str = ft_quotes_clean(export_str);

    equal = ft_strchr(export_str, '=');
    if (equal && equal[1] != '\0')
    {
        variable = ft_strndup(export_str, equal - export_str);
        value = ft_strdup(equal + 1);
        tokens[i++] = variable;
        tokens[i++] = value;
    }
    else
    {
        tokens[i++] = ft_strdup(export_str);
        tokens[i++] = NULL;
    }
    tokens[i] = NULL;
    return tokens;
}

void ft_export(t_program *prgm, char *arg)
{
    int i;
    char *variable;
    char *value = NULL;
    char **tokens;

    if (!arg || !arg[0])
    {
        return;
    }
    tokens = ft_export_tokenize(arg);
    if (tokens && tokens[0])
    {
        variable = tokens[0];
        value = tokens[1] ? tokens[1] : NULL;
        if (value && value[0] == '\0')
            value = NULL;
        if (!ft_is_valid_export(variable))
        {
            free(variable);
            free(value);
            free(tokens);
            return;
        }
    }
    else
    {
        variable = ft_strdup(arg);
        value = NULL;
        if (!ft_is_valid_export(variable))
        {
            free(variable);
            free(value);
            free(tokens);
            return;
        }
    }
    ft_export_list(prgm, variable, value);
    free(variable);
    free(value);
    free(tokens);
}

// void	ft_export(t_program *prgm, char **arg)
// {
// 	int		i;
// 	char	*variable;
// 	char	*value;
// 	char	*tmp;
	
// 	i = 1;
// 	if (!arg[1])
// 	{
// 		t_environment *tmp;
		
// 		tmp = prgm->exp_list;
// 		while (tmp)
// 		{
// 			printf("declare -x %s\n", tmp->variable);
// 			tmp = tmp->next;
// 		}
// 		return;
// 	}
// 	while(arg[i])
// 	{
// 		if (!ft_is_valid_export(arg[i]))
// 		{
// 			i++;
// 			continue;
// 		}
// 		if (strchr(arg[i], '='))
// 		{
// 			variable = ft_strndup(arg[i], (strchr(arg[i], '=') - arg[i]));
// 			if (arg[i + 1] == NULL || arg[i + 1][0] == ' ' || arg[i + 1][0] == '\t')
// 			{
// 				variable = ft_strdup(arg[i]);
// 				value = NULL;
// 			}
// 			else
// 			{
// 				tmp = ft_quotes_clean(arg[i]);
// 				value = ft_strdup(strchr(tmp, '=') + 1);
// 			}
// 		}
// 		else
// 		{
// 			variable = ft_strdup(arg[i]);
// 			value = NULL;
// 		}
		
// 		ft_export_list(prgm, variable, value);
// 		free(variable);
// 		if (value)
// 			free(value);
// 		i++;
// 	}
// }


/*
void ft_unset(t_program *prgm, char *arg)
{
    int i;
    char *variable;
    char **tokens;

    if (!arg || !arg[0])
    {
        return;  // Geçerli bir argüman yoksa, fonksiyonu sonlandır
    }

    tokens = ft_unset_tokenize(arg);  // unset komutunu tokenize et
    if (tokens && tokens[0])
    {
        variable = tokens[0];

        // Geçerliliği kontrol et (export kontrolüne benzer şekilde)
        if (!ft_is_valid_export(variable))
        {
            free(variable);
            free(tokens);
            return;
        }

        // Değişkeni silmek için listeden çıkar
        t_environment *prev = NULL;
        t_environment *current = prgm->exp_list;
        while (current)
        {
            if (strcmp(current->variable, variable) == 0)
            {
                // Değişken bulundu ve siliniyor
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

        // Eğer env_list'ten de silmek gerekirse (benzer şekilde)
        prev = NULL;
        current = prgm->env_list;
        while (current)
        {
            if (strcmp(current->variable, variable) == 0)
            {
                // Değişken bulundu ve siliniyor
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

    free(variable);
    free(tokens);
}
*/