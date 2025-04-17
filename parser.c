/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bekarada <bekarada@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:47:42 by bekarada          #+#    #+#             */
/*   Updated: 2025/03/23 16:10:27 by bekarada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_is_builtin(char *str)
{
	if(ft_strcmp(str, "echo") == 0)
		return (1);
	else if (ft_strcmp(str, "cd") == 0)
		return(1);
	else if (ft_strcmp(str, "pwd") == 0)
		return (1);
	else if (ft_strcmp(str, "export") == 0)
		return (1);
	else if (ft_strcmp(str, "unset") == 0)
		return (1);
	else if (ft_strcmp(str, "env") == 0)
		return (1);
	else if (ft_strcmp(str, "exit") == 0)
		return (1);
	else 
		return (0);
}

void	ft_pipe_parser(t_program *prgm)
{
	
}

void	ft_basic_parser(t_program *prgm)
{
	char	*str;

	str = prgm->tokens->value;
	if(ft_is_builtin(str))
		ft_executor(prgm, 1);
	else
		ft_executor(prgm, 0);
}

/*int		ft_token_check(t_token *token, int pipe_flag)
{
	char *str;

	while (token)
	{
		str = token->value;
		if (ft_strcmp(str, "|") == 0 && pipe_flag == 0)
			return (1);
		token = token->next;
	}
	return (0);
}

void	ft_parser(t_program *prgm)
{
	if (!prgm || !prgm->tokens)
		return;
	if (ft_token_check(prgm->tokens, prgm->tokens->pipe_flag) == 1)
	{
		printf("45678\n");
		ft_pipe_parser(prgm);
	}
	if (ft_token_check(prgm->tokens, prgm->tokens->pipe_flag) == 0)
	{
		printf("123\n");
		ft_basic_parser(prgm);	
	}	
}*/
void	ft_parser(t_program *prgm)
{
	t_token *current;

	if (!prgm || !prgm->tokens)
		return;

	// Tüm tokenlar arasında pipe_flag 1 olan var mı?
	current = prgm->tokens;
	while (current)
	{
		if (current->pipe_flag == 1) // Eğer herhangi bir token'da pipe varsa
		{
			printf("45678\n");
			ft_pipe_parser(prgm);
			return; // Pipe varsa direkt çık, diğer kontrolü yapmaya gerek yok
		}
		current = current->next;
	}

	// Eğer hiçbir token'ın pipe_flag'i 1 değilse, basic parser'a git
	printf("123\n");
	ft_basic_parser(prgm);
}
