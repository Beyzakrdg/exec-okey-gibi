/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elikavak <elikavak@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:09:32 by elikavak          #+#    #+#             */
/*   Updated: 2025/03/23 15:04:14 by elikavak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <unistd.h>

void display_execution_error(char *argument, char *message)
{
    write(2, "Execution error: ", 17);
    write(2, argument, strlen(argument));
    write(2, message, strlen(message));
    write(2, "\n", 1);
}

static int	is_valid_numeric_argument(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[i])
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	exit_with_error_message(char *msg, int status)
{
	write(2, msg, ft_strlen(msg));
	exit(status);
}

static void	remove_quotes_from_value(char *value)
{
	char	*new_value;
	int		i;
	int		j;

	if (!value)
		return ;
	new_value = (char *)malloc(ft_strlen(value) + 1);
	if (!new_value)
		return ;
	i = 0;
	j = 0;
	while (value[i])
	{
		if (value[i] != '"' && value[i] != '\'')
			new_value[j++] = value[i];
		i++;
	}
	new_value[j] = '\0';
	ft_strcpy(value, new_value);
	free(new_value);
}

static void	exit_with_status(t_program *program, int status)
{
	program->status_code = status;
	exit_with_error_message("exit\n", status);
}

void	ft_exit(t_program *program)
{
	t_token	*argument_token;
	char	*argument_value;

	if (!program->tokens || !program->tokens->next)
		exit_with_status(program, 0);
	argument_token = program->tokens->next;
	if (!argument_token)
		exit_with_status(program, 0);
	argument_value = ft_strdup(argument_token->value);
	remove_quotes_from_value(argument_value);
	if (!is_valid_numeric_argument(argument_value))
	{
		display_execution_error(argument_token->value,
			" numeric argument required");
		free(argument_value);
		exit_with_status(program, 255);
	}
	if (argument_token->next != NULL)
	{
		free(argument_value);
		display_execution_error(argument_token->value, " too many arguments");
		program->status_code = 1;
		return ;
	}
	program->status_code = ft_atoi(argument_value) % 256;
	free(argument_value);
	exit_with_status(program, program->status_code);
}
