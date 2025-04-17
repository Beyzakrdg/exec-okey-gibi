/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elikavak <elikavak@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 15:22:29 by bekarada          #+#    #+#             */
/*   Updated: 2025/03/23 15:14:13 by elikavak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_environment	*get_env_var(t_environment *env_list, char *var_name)
{
	while (env_list)
	{
		if (strcmp(env_list->variable, var_name) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

void	update_env_var(t_environment **env_list, char *var, char *new_val)
{
	t_environment	*var_node;

	var_node = get_env_var(*env_list, var);
	if (var_node)
	{
		free(var_node->value);
		var_node->value = ft_strdup(new_val);
	}
	else
	{
		var_node = malloc(sizeof(t_environment));
		if (!var_node)
			return ;
		var_node->variable = ft_strdup(var);
		var_node->value = ft_strdup(new_val);
		var_node->next = *env_list;
		*env_list = var_node;
	}
}

char	*get_cd_argument(t_token *tokens)
{
	if (!tokens || !tokens->next)
		return (NULL);
	return (tokens->next->value);
}

static char	*resolve_cd_path(t_program *prgm, char *arg)
{
	t_environment	*env;

	if (!arg)
	{
		env = get_env_var(prgm->env_list, "HOME");
		if (!env || !env->value)
		{
			printf("minishell: cd: HOME not set\n");
			return (NULL);
		}
		return (env->value);
	}
	if (strcmp(arg, "-") == 0)
	{
		env = get_env_var(prgm->env_list, "OLDPWD");
		if (!env || !env->value)
		{
			printf("minishell: cd: OLDPWD not set\n");
			return (NULL);
		}
		printf("%s\n", env->value);
		return (env->value);
	}
	return (arg);
}

void	ft_cd(t_program *prgm)
{
	char		cwd[1024];
	char		*path;
	t_environment	*oldpwd_env;

	if (!prgm || !prgm->tokens)
		return ;
	oldpwd_env = get_env_var(prgm->env_list, "PWD");
	path = resolve_cd_path(prgm, get_cd_argument(prgm->tokens));
	if (!path || chdir(path) != 0)
	{
		perror("minishell: cd");
		return ;
	}
	if (oldpwd_env)
		update_env_var(&prgm->env_list, "OLDPWD", oldpwd_env->value);
	if (getcwd(cwd, sizeof(cwd)))
		update_env_var(&prgm->env_list, "PWD", cwd);
}


/*static void	update_env_var(t_environment **env, char *key, char *val)
{
	t_environment	*cur;

	cur = *env;
	while (cur)
	{
		if (ft_strcmp(cur->variable, key) == 0)
		{
			free(cur->value);
			cur->value = ft_ft_strdup(val);
			return ;
		}
		cur = cur->next;
	}
	ft_create_env(NULL, (char *[]){key, val, NULL}, env);
}

static void	update_pwd(t_program *prgm, char *oldpwd)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("cd");
		prgm->status_code = 1;
		return ;
	}
	update_env_var(&(prgm->env_list), "OLDPWD", oldpwd);
	update_env_var(&(prgm->env_list), "PWD", new_pwd);
	update_env_var(&(prgm->exp_list), "OLDPWD", oldpwd);
	update_env_var(&(prgm->exp_list), "PWD", new_pwd);
	free(new_pwd);
}

static void	change_directory(t_program *prgm, char *path)
{
	if (chdir(path) != 0)
	{
		perror("cd");
		prgm->status_code = 1;
	}
	else
		prgm->status_code = 0;
}

static void	change_to_home(t_program *prgm)
{
	t_environment	*env;
	char			*home;

	env = prgm->env_list;
	home = NULL;
	while (env)
	{
		if (ft_strcmp(env->variable, "HOME") == 0)
		{
			home = env->value;
			break ;
		}
		env = env->next;
	}
	if (home)
		change_directory(prgm, home);
	else
	{
		write(2, "cd: HOME not set\n", 17);
		prgm->status_code = 1;
	}
}

void	ft_cd(t_program *prgm)
{
	char	*oldpwd;
	t_token	*token;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("cd");
		prgm->status_code = 1;
		return ;
	}

	token = prgm->tokens;

	if (!token)
	{
		write(2, "cd: No arguments given\n", 23);
		prgm->status_code = 1;
		return;
	}

	if (ft_strcmp(token->value, "~") == 0)
		change_to_home(prgm);
	else
	{
		change_directory(prgm, token->value);
		if (prgm->status_code == 0)
			update_pwd(prgm, oldpwd);
	}

	free(oldpwd);
}*/
