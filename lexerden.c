/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elikavak <elikavak@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 20:38:24 by elikavak          #+#    #+#             */
/*   Updated: 2025/03/28 20:38:24 by elikavak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// echo"ebrar" hatası hariç okey duruyo
// $? eklendi

#include "minishell.h"

void	print_tokens(t_token *token) //silinecek
{
	while (token)
	{
		printf("Token: %s      %d\n", token->value, token->dollar_flag);
		token = token->next;
	}
}

char	**ft_free_split(char **arr, int i)
{
	while (i > 0)
		free(arr[--i]);
	free(arr);
	return (NULL);
}

char	*get_env_value(const char *var)
{
	char	*value;

	value = getenv(var);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

char	*expand_variable(char *input, int *index, int last_exit_code)
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
void	ft_expand_and_set_token(t_token *new_token, char *add_token, int k, int last_exit_code)
{
	char	*expanded_value;
	int		index;

	if (k != 0)
	{
		new_token->value = add_token;
		return ;
	}
	index = 0;
	expanded_value = expand_variable(add_token, &index, last_exit_code);
	if (!expanded_value)
	{
		printf("Error: Memory allocation failed in expand_variable\n");
		free(new_token);
		return ;
	}
	free(add_token);
	new_token->value = expanded_value;
}

//ilk hali
/*
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
*/

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

void	ft_add_token(t_token **token, char *add_token, int k, int pipe_flag, int status_code)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		exit(1);
	new_token->dollar_flag = k;
	new_token->pipe_flag = pipe_flag;
	new_token->next = NULL;
	ft_expand_and_set_token(new_token, add_token, k, status_code);
	ft_append_token(token, new_token);
}

int ft_all_pipe(char *str)
{
	int		i;
	int		q;
	i = 0;
	q = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == '"' || str[i] == '\''))
		{
			i++;
			q = 1;
		}	
		if (str[i] != '|' && q == 1)
			return (1);
		i++;
		while (str[i] && (str[i] == '"' || str[i] == '\''))
		{
			i++;
			q = 0;
		}
	}
	return (0);		
}

/*int ft_all_pipe(char *str)
{
	int i = 0;
	int inside_quotes = 0;
	char quote_type = '\0';

	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			if (!inside_quotes)
			{
				inside_quotes = 1;
				quote_type = str[i];
			}
			else if (inside_quotes && str[i] == quote_type)
				inside_quotes = 0;
		}
		else if (str[i] != '|' && !inside_quotes)  // Eğer tırnak içinde değilsek ve farklı bir karakter geldiyse
			return (1);
		i++;
	}
	return (0);  // Eğer tüm karakterler '|' ise ve başka bir şey yoksa 0 döndür
}*/


int	ft_pipe_flag_check(char *token)
{
	int	i;
	int	in_quotes;

	i = 0;
	in_quotes = 0;
	while (token[i])
	{
		if (token[i] == '\"' || token[i] == '\'') // Tırnak açma/kapatma kontrolü
			in_quotes = !in_quotes; // Tırnak içindeysek, durum değiştir

		if (token[i] == '|' && !in_quotes) // Eğer boru işareti tırnak içinde değilse
			return (1);

		i++;
	}
	return (0);
}

/*int		ft_pipe_flag_check(char *token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		if (token[i] == '\"')
		{
			if (ft_strcmp(token + i, "|") == 0)
				return (1);
		}
		i++;
	}
	return (0);
}*/

int		ft_token_pipe(char *token)
{
	int	i;

	i = 0;
	while (token[i] == '|')
		i++;
	if (token[i] == '\0')
		return (0);
	return (1);
}

int ft_pipe_count(char *token)
{
	int i;
	int	p_c;

	i = 0;
	p_c = 0;
	while(token[i])
	{
		if (token[i] == '|')
			p_c++;
		i++;
	}
	return (p_c);
}

void	ft_where_pipe(char *token, int *j)
{
	int i;
	
	i = 0;
	while (token[i])
	{
		if (token[i] == '|')
		{
			*j = i;
			return ;
		}
		i++;
	}
	*j = *j + i;
}

char	**ft_pipe_split(char *token) // norm
{
	char	**str;
	int		i;
	int		pos;
	int		pipe_count;

	pipe_count = ft_pipe_count(token);
	str = (char **)malloc(sizeof(char *) * (2 * pipe_count + 2));
	if (!str)
		return (NULL);
	i = 0;
	pos = 0;
	while (i < (2 * pipe_count + 1))
	{
		int	start = pos;
		ft_where_pipe(token + pos, &pos);
		str[i] = ft_strndup(token + start, pos - start);
		if (!str[i])
			return (ft_free_split(str, i));
		i++;
		if (token[pos] == '|')
		{
			str[i] = ft_strdup("|");
			if (!str[i])
				return (ft_free_split(str, i));
			pos++;
			i++;
		}
	}
	str[i] = NULL;
	return (str);
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
	// Kelimenin sonuna kadar ilerle, tırnak görünce dur
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '"' && input[*i] != '\'')
		(*i)++;

	// Eğer tırnak geldiyse, sadece ilk kelimeyi döndür
	if (input[*i] == '"' || input[*i] == '\'')
		return (ft_substr(input, start, *i - start));

	// Normal kelimeyi döndür
	return (ft_substr(input, start, *i - start));
}

/*char	*ft_handle_word(char *input, int *i, int start)
{
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '"' && input[*i] != '\'')
		(*i)++;
	return (ft_substr(input, start, *i - start));
}*/

void	ft_trim_and_add_token(t_token **token_list, char *token, int status_code)
{
	size_t	len;
	char	*trimmed;
    int     dollar_flag = 0;

	len = ft_strlen(token);
    dollar_flag = dollar_syntax_check(token);
	if (len >= 2 && (token[0] == '"' && token[len - 1] == '"') || (token[0] == '\'' && token[len - 1] == '\''))
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
		ft_add_token(token_list, token, dollar_flag, 0, status_code);
}

void	ft_trim_and_add_token_pipe(t_token **token_list, char *token, int status_code)
{
	size_t	len;
	int	pipe_flag;
	char	*trimmed;
    int     dollar_flag = 0;

	len = ft_strlen(token);
	pipe_flag = ft_pipe_flag_check(token);
    dollar_flag = dollar_syntax_check(token);
    printf("all pipe %d\n", ft_all_pipe(token));
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
		ft_add_token(token_list, token, dollar_flag, pipe_flag, status_code);
}

void	ft_handle_pipes(t_token **token_list, char *token, int status_code)
{
	int		pipe_flag;
	int		pipe_count;
	char	**tok_p;
    int     dollar_flag = 0;
	int		j;

	j = 0;
	pipe_flag = ft_pipe_flag_check(token);
    printf("pipe flag %d\n", pipe_flag);
	if (pipe_flag == 1 && ft_token_pipe(token))
	{
		pipe_count = ft_pipe_count(token);
		tok_p = ft_pipe_split(token);
		while ((2 * pipe_count + 1) > j)
		{
            //printf("tok: %s\n",tok_p[j]);
            dollar_flag = dollar_syntax_check(tok_p[j]);
            //printf("dollar pipe %d\n", dollar_flag);
			ft_add_token(token_list, tok_p[j], dollar_flag, pipe_flag, status_code);
			j++;
		}
		return ;
	}
    printf("aaaa\n");
	ft_trim_and_add_token_pipe(token_list, token, status_code);
}

void	ft_pipe_lexer(char *input, t_token **token_list, int quote_flag, int status_code)
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
		//k = dollar_syntax_check(input + i);
		if (input[i] == '"' || input[i] == '\'')
			token = ft_handle_quotes(input, &i);
		else
			token = ft_handle_word(input, &i, start);
		if (!token)
		{
			printf("Error: Memory allocation failed\n");
			return ;
		}
        printf("pipe BEFORE handle %s\n", token);
		ft_handle_pipes(token_list, token,status_code);
	}
	print_tokens((*token_list)); // silinecek
}

void	ft_lexer(char *input, t_token **token_list, int status_code)
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
		ft_pipe_lexer(input, token_list, 0, status_code);
		return ;
	}
	if ((*token_list)->redirect_flag != 0) 
	{
		ft_redirect_lexer(input, token_list, 0, status_code);
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
		ft_trim_and_add_token(token_list, token, status_code);
	}
	print_tokens((*token_list)); // silinecek
}
