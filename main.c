/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bekarada <bekarada@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:39:05 by bekarada          #+#    #+#             */
/*   Updated: 2025/03/23 16:14:00 by bekarada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_init_struct(t_program *prgm)
{
	if (!prgm)
		return ;
	prgm->id = 0;
	prgm->quote_state = 0;
	prgm->single_quote = 0;
	prgm->double_quote = 0;
	prgm->pipe_count = 0;
	prgm->status_code = 0;
	prgm->env_len = 0;
	prgm->input_program = NULL;
	prgm->parsed_program = NULL;
	prgm->tokens = NULL;
	prgm->full_path = NULL;
	prgm->env_list = NULL;
}

static void	ft_free_input(t_program *prgm)
{
	free(prgm->input_program);
	prgm->input_program = NULL;
}

static void	ft_start_prgm(t_program *prgm)
{
	while (1)
	{
		prgm->input_program = readline("tinyshell> ");
		if (!prgm->input_program)
		{
			printf("exit\n");
			free(prgm);
			exit(0);
		}
		if (history_check(prgm) == 1)
			add_history(prgm->input_program);
		if (handle_quotes(prgm) || pipe_check(prgm->input_program, 0, 0)
			|| validate_redirects(prgm))
		{
			ft_free_input(prgm);
			continue;
		}
		ft_lexer(prgm->input_program, &(prgm->tokens), prgm->status_code);
		ft_parser(prgm);
		free(prgm->tokens);
		prgm->tokens = NULL;
		ft_free_input(prgm);
	}
	free(prgm);
}


int	main(int argc, char *argv[], char **env)
{
	t_program	*prgm;

	(void)argv;
	if (argc != 1)
	{
		write(2, "This program does not accept arguments\n", 40);
		return (1);
	}
	prgm = (t_program *)malloc(sizeof(t_program));
	if (!prgm)
		return (1);
	ft_init_struct(prgm);
	ft_signal();
	ft_create_env(prgm, env, &prgm->env_list);
	ft_create_env(prgm, env, &prgm->exp_list);
	ft_start_prgm(prgm);
	free(prgm->input_program);
	free(prgm);
}

/*static void	ft_init_struct(t_program *prgm)
{
	if (!prgm)
		return ;
	prgm->id = 0;
	prgm->quote_state = 0;
	prgm->single_quote = 0;
	prgm->double_quote = 0;
	prgm->pipe_count = 0;
	prgm->status_code = 0;
	prgm->env_len = 0;
	prgm->input_program = NULL;
	prgm->parsed_program = NULL;
	prgm->tokens = NULL;
	prgm->full_path = NULL;
	prgm->env_list = NULL;
}

static void	ft_start_prgm(t_program *prgm)
{
	while (1)
	{
		prgm->input_program = readline("tinyshell> ");
		if (!prgm->input_program)
		{
			printf("exit\n");
			free(prgm->input_program);
			free(prgm);
			exit(0);
		}
		if (prgm->input_program && history_check(prgm) == 1)
			add_history(prgm->input_program);
		if (handle_quotes(prgm) || pipe_check(prgm->input_program, 0, 0) || validate_redirects(prgm))
		{
			free(prgm->input_program);
			prgm->input_program = NULL;
			continue;
    	}
		ft_lexer(prgm->input_program, &(prgm->tokens));
		ft_parser(prgm);
		if (prgm->tokens)
		{
			free(prgm->tokens);
			prgm->tokens = NULL;
		}
		if (prgm->input_program)
		{
			free(prgm->input_program);
			prgm->input_program = NULL;
		}
	}
	if (prgm)
    {
        free(prgm);
        prgm = NULL;
    }
}

int main(int argc, char *argv[], char **env)
{
	t_program *prgm;

	(void)argv;

	if (argc != 1)
	{
		write(2, "This program does not accept arguments\n", 40);
		return (1);
	}
	prgm = (t_program *)malloc(sizeof(t_program));
	if (!prgm)
		return (1);
	ft_init_struct(prgm);
	ft_signal();
	ft_create_env(prgm, env, &prgm->env_list);
	ft_create_env(prgm, env, &prgm->exp_list);
	ft_start_prgm(prgm);
	free(prgm->input_program);
	free(prgm);
}
*/