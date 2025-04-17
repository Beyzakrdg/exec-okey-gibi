/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bekarada <bekarada@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:51:12 by elikavak          #+#    #+#             */
/*   Updated: 2025/03/25 14:50:37 by bekarada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exec_built(t_program *prgm)
{
	if (!prgm || !prgm->tokens)
	{
		printf("Error: Invalid tokens\n");
		return;
	}
	if(ft_strcmp(prgm->tokens->value, "echo") == 0)
		ft_echo(prgm);
	else if (ft_strcmp(prgm->tokens->value, "cd") == 0)
		ft_cd(prgm);
	else if (ft_strcmp(prgm->tokens->value, "pwd") == 0)
		ft_pwd(prgm);
	else if (ft_strcmp(prgm->tokens->value, "export") == 0)
	{
		if (prgm->tokens->next)
		{
			char *combined = reconstruct_export_argument(prgm->tokens->next);
			if (combined)
			{
				ft_export(prgm, combined);
				free(combined);
			}
		}
		else
		{
			
			t_environment *tmp = prgm->exp_list;
			print_sorted_export(tmp);
		}
	}
	else if (ft_strcmp(prgm->tokens->value, "unset") == 0)
	{
		printf("ee\n");
		printf("prgm->tokens->value: %s\n", prgm->tokens->value);
	
		// Token sayısını say
		int count = 0;
		t_token *temp = prgm->tokens;
		while (temp)
		{
			count++;
			temp = temp->next;
		}
	
		// char **args belleği ayır
		char **args = malloc(sizeof(char *) * (count + 1));
		if (!args)
			return;
	
		// Token değerlerini diziye doldur
		temp = prgm->tokens;
		int i = 0;
		while (temp)
		{
			args[i++] = ft_strdup(temp->value);
			temp = temp->next;
		}
		args[i] = NULL;
	
		// unset fonksiyonunu çağır
		ft_unset(prgm, args);
	
		// Belleği temizle
		i = 0;
		while (args[i])
			free(args[i++]);
		free(args);
	}
	
	else if (ft_strcmp(prgm->tokens->value, "env") == 0)
		ft_env(prgm);
	else if (ft_strcmp(prgm->tokens->value, "exit") == 0)
		ft_exit(prgm);
}

void	ft_executor(t_program *prgm, int exec_flag)
{
	if (exec_flag == 1)
		ft_exec_built(prgm);
	else
		ft_exec_not_built(prgm);
}
