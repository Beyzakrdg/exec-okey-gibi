/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elikavak <elikavak@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:45:49 by bekarada          #+#    #+#             */
/*   Updated: 2025/03/23 13:58:14 by elikavak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_create_env(t_program *prgm, char **env, t_environment **env_list)
{
	int				i;
	char			*key;
	char			*value;
	char			*delimiter;
	t_environment	*new_env;

	i = 0;
	while (env[i])
	{
		delimiter = ft_strchr(env[i], '=');
		if (delimiter)
		{
			key = ft_strndup(env[i], delimiter - env[i]);
			value = ft_strdup(delimiter + 1);
			if (!key || !value)
			{
				free(key);
				free(value);
				perror("malloc");
				prgm->status_code = 1;
				return ;
			}
			new_env = (t_environment *)malloc(sizeof(t_environment));
			if (!new_env)
			{
				free(key);
				free(value);
				perror("malloc");
				prgm->status_code = 1;
				return ;
			}
			new_env->variable = key;
			new_env->value = value;
			new_env->next = *env_list;
			*env_list = new_env;
		}
		i++;
	}
}

void	ft_len_env(t_program *prgm)
{
	int	i;
	t_environment *env;

	i = 0;
	env = prgm->env_list;
	while (env)
	{
		i++;
		env = env->next;
	}
	prgm->env_len = i;
}

static void	print_env_reverse(t_environment *env_list, int len)
{
    t_environment **env_array;
    int i;

    env_array = (t_environment **)malloc(sizeof(t_environment *) * len);
    if (!env_array)
    {
        perror("malloc");
        return ;
    }
    i = 0;
    while (env_list)
    {
        env_array[i++] = env_list;
        env_list = env_list->next;
    }
    while (i > 0)
    {
        i--;
        write(1, env_array[i]->variable, ft_strlen(env_array[i]->variable));
        write(1, "=", 1);
        write(1, env_array[i]->value, ft_strlen(env_array[i]->value));
        write(1, "\n", 1);
    }
    free(env_array);
}


void	ft_env(t_program *prgm) 
{
    ft_len_env(prgm); 
    print_env_reverse(prgm->env_list, prgm->env_len);
}

void free_env_list(t_environment *env_list)
{
    t_environment *temp;

    while (env_list)
    {
        temp = env_list;
        env_list = env_list->next;
        free(temp->variable);
        free(temp->value);
        free(temp);
    }
}

/// 
/// 

void sort_env(t_environment **env, int size)
{
	int i;
	int j;

	i = 0;
	while (i < size - 1)
	{
		j = i + 1;
		while (j < size)
		{
			if (strcmp(env[i]->variable, env[j]->variable) > 0)
			{
				t_environment *tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

int env_list_size(t_environment *head)
{
    int size = 0;
    while (head)
    {
        size++;
        head = head->next;
    }
    return size;
}

t_environment **copy_env_to_array(t_environment *head, int size)
{
    t_environment **array = malloc(sizeof(t_environment *) * size);
    int i = 0;
    while (head)
    {
        array[i++] = head;
        head = head->next;
    }
    return array;
}


void print_sorted_export(t_environment *exp_list)
{
	int i;
    int size = env_list_size(exp_list);

	i = 0;
    t_environment **array = copy_env_to_array(exp_list, size);
	
	
    sort_env(array, size);
	printf("deneme\n");
    while (i < size)
    {
        if (array[i]->value)
            printf("declare -x %s=\"%s\"\n", array[i]->variable, array[i]->value);
        else
            printf("declare -x %s\n", array[i]->variable);
		i++;
    }
    free(array);
}
