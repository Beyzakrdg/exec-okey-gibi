/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirecttoken.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bekarada <bekarada@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:01:12 by bekarada          #+#    #+#             */
/*   Updated: 2025/04/15 17:12:52 by bekarada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_variable_re(char *input, int *index, int last_exit_code)
{
	int		start;
	int		len;
	char	*var_name;
	char	*expanded_value;

	// $? özel durumu
	if (input[*index + 1] == '?')
	{
		(*index) += 2;
		return (ft_itoa(last_exit_code)); // Çıkış kodunu stringe çevir ve döndür
	}

	start = *index + 1;
	len = 0;
	while (ft_isalnum(input[start + len]) || input[start + len] == '_')
		len++;

	if (len == 0)
		return (ft_strdup("$")); // Geçersiz değişkense sadece "$" döndür

	var_name = ft_substr(input, start, len);
	*index += len;
	expanded_value = get_env_value(var_name);
	free(var_name);

	return (expanded_value);
}

void	ft_append_token_re(t_token **token, t_token *new_token)
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

void	ft_add_token_re(t_token **token, char *add_token, int k, int status_code)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		exit(1);
	new_token->dollar_flag = k;
	new_token->pipe_flag = 0;  // Pipe flag kaldırıldı
	new_token->next = NULL;
	ft_expand_and_set_token(new_token, add_token, k, status_code);
	ft_append_token_re(token, new_token);
}

void	ft_trim_and_add_token_redirect(t_token **token_list, char *token, int status_code)
{
	size_t	len;
	char	*trimmed;
    int     dollar_flag = 0;

	len = ft_strlen(token);
    dollar_flag = dollar_syntax_check(token);
	if (len >= 2 && ((token[0] == '"' && token[len - 1] == '"') || (token[0] == '\'' && token[len - 1] == '\'')))
	{
		trimmed = ft_substr(token, 1, len - 2);
		free(token);
		token = trimmed;
		if (!token)
		{
			printf("Error: Memory allocation failed while trimming quotes\n");
			return ;
		}
	}
	if (ft_strcmp(token, "") != 0)
		ft_add_token_re(token_list, token, dollar_flag, status_code);
}


int	ft_control_redirect(t_program *prgm, char *input, int status_code)
{
	if (strcmp(prgm->input_program, "<") == 0 || strcmp(prgm->input_program, "<<") == 0 || strcmp(prgm->input_program, ">") == 0 || strcmp(prgm->input_program, ">>") == 0)
	{
		ft_trim_and_add_token_redirect(&prgm->tokens, input, status_code);
		return(1);
	}
	return(0);
}

int is_even_number_of_double_quotes(const char *str)
{
	int i = 0;
	int count = 0;

	while (str[i])
	{
		if (str[i] == '\"')
			count++;
		i++;
	}
	return (count % 2 == 0);
}


int is_even_number_of_single_quotes(const char *str)
{
	int i = 0;
	int count = 0;

	while (str[i])
	{
		if (str[i] == '\'')
			count++;
		i++;
	}
	return (count % 2 == 0);
}


int	ft_redirect_flag_check(char *token)
{
	int	i = 0;
	int	in_single_quote = 0;
	int	in_double_quote = 0;

	while (token[i])
	{
		if (token[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (token[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		if (!in_single_quote && !in_double_quote)
		{
			if ((token[i] == '>' && token[i + 1] == '>') ||
				(token[i] == '<' && token[i + 1] == '<'))
				return (1);
			else if (token[i] == '>' || token[i] == '<')
				return (1);
		}
		i++;
	}
	if (in_single_quote || in_double_quote)
		return (-1);

	return (0);
}


void	ft_handle_redirect(t_token **token_list, char *token, int status_code)
{
	int		dollar_flag = 0;
	int		j = 0;
	int		double_quotes_even = is_even_number_of_double_quotes(token);
	int		single_quotes_even = is_even_number_of_single_quotes(token);

	(*token_list)->redirect_flag = ft_redirect_flag_check(token);
	if ((*token_list)->redirect_flag == 1 && double_quotes_even && single_quotes_even)
	{
		ft_trim_and_add_token_redirect(token_list, token, status_code);
		return ;
	}
	ft_trim_and_add_token_redirect(token_list, token, status_code);
}

char	*ft_handle_word_re(char *input, int *i, int start)
{
	char *ret;

	if (input[*i] == '"' || input[*i] == '\'')
	{
		char quote = input[*i];
		start = ++(*i);
		while (input[*i] && input[*i] != quote)
			(*i)++;
		(*i)++;
		return (ft_substr(input, start, *i - start - 1));
	}
	if ((input[*i] == '>' || input[*i] == '<'))
	{
		if (input[*i] == input[*i + 1]) // >> veya <<
		{
			ret = ft_substr(input, *i, 2);
			*i += 2;
		}
		else
		{
			ret = ft_substr(input, *i, 1);
			*i += 1;
		}
		return ret;
	}

	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '"' && input[*i] != '\'' && input[*i] != '<' && input[*i] != '>')
		(*i)++;

	return (ft_substr(input, start, *i - start));
}

void	ft_redirect_lexer(char *input, t_token **token_list, int quote_flag, int status_code)
{
	int		i;
	int		start;
	int		k;
	char	*token;

	i = 0;
	if (!input || !token_list)
	{
		printf("Error: NULL input or token_list\n");
		return ;
	}
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		start = i;
		token = ft_handle_word_re(input, &i, start);
		if (!token)
		{
			printf("Error: Memory allocation failed\n");
			return ;
		}
        ft_handle_redirect(token_list, token, status_code);
		free(token);
	}
	print_tokens((*token_list)); // silinecek
}
