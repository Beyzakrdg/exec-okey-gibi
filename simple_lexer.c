/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_lexer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elikavak <elikavak@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 21:33:11 by elikavak          #+#    #+#             */
/*   Updated: 2025/03/28 21:33:11 by elikavak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(const char *var)
{
	char	*value;

	value = getenv(var);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

char	*expand_variable(char *input, int *index)
{
	int		start;
	int		len;
	char	*var_name;
	char	*expanded_value;

	start = *index + 1;
	len = 0;
	while (ft_isalnum(input[start + len]) || input[start + len] == '_')
		len++;
	if (len == 0)
		return (ft_strdup("$"));
	var_name = ft_substr(input, start, len);
	*index += len;
	expanded_value = get_env_value(var_name);
	free(var_name);
	return (expanded_value);
}

void	ft_expand_and_set_token(t_token *new_token, char *add_token, int k)
{
	char	*expanded_value;
	int		index;

	if (k != 0)
	{
		new_token->value = add_token;
		return ;
	}
	index = 0;
	expanded_value = expand_variable(add_token, &index);
	if (!expanded_value)
	{
		printf("Error: Memory allocation failed in expand_variable\n");
		free(new_token);
		return ;
	}
	free(add_token);
	new_token->value = expanded_value;
}

void	ft_append_token(t_token **token, t_token *new_token)
{
	t_token	*last;

	if (!(*token))
	{
		*token = new_token;
		return ;
	}
	last = *token;
	while (last->next)
		last = last->next;
	last->next = new_token;
}

void	ft_add_token(t_token **token, char *add_token, int k, int pipe_flag)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		exit(1);
	new_token->dollar_flag = k;
	new_token->pipe_flag = pipe_flag;
	new_token->next = NULL;
	ft_expand_and_set_token(new_token, add_token, k);
	ft_append_token(token, new_token);
}

char	*ft_handle_quotes(char *input, int *i)
{
	int		start;
	char	quote_flag;
	char	*token;

	quote_flag = input[*i]; // İlk tırnağı al
	start = (*i)++; // Tırnağın başlangıç indeksini belirle ve ilerlet
	while (input[*i] && input[*i] != quote_flag)
		(*i)++;
	if (input[*i] == quote_flag)
		(*i)++; // Kapanış tırnağını da dahil et
	token = ft_substr(input, start, *i - start); // Tırnakları içeren alt diziyi al
	return (token);
}

char	*ft_handle_word(char *input, int *i, int start)
{
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '"' && input[*i] != '\'')
		(*i)++;
	return (ft_substr(input, start, *i - start));
}

void	ft_lexer(char *input, t_token **token_list)
{
	int		i;
	int		start;
	char	*token;

	i = 0;
	if (!input || !token_list)
	{
		printf("Error: NULL input or token_list\n");
		return ;
	}
	if (ft_strchr(input, '|'))
	{
		ft_pipe_lexer(input, token_list, 0);
		return ;
	}
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		start = i;
		if (input[i] == '"' || input[i] == '\'')
			token = ft_handle_quotes(input, &i);
		else
			token = ft_handle_word(input, &i, start);
		if (!token)
		{
			printf("Error: Memory allocation failed\n");
			return ;
		}
		ft_trim_and_add_token(token_list, token);
	}
	print_tokens((*token_list)); // silinecek
}