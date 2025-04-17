/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bekarada <bekarada@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 14:05:56 by bekarada          #+#    #+#             */
/*   Updated: 2025/03/22 14:35:49 by bekarada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_tokens(t_token *token)
{
    while (token)
    {
        printf("Token: %s      %d\n", token->value, token->dollar_flag);
        token = token->next;
    }
}
char	*expand_variable(char *input, int *index);

void	ft_add_token(t_token **token, char *add_token, int k, int pipe_flag)
{
	t_token	*new_token;
	t_token	*last;
	char	*expanded_value;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (new_token == NULL)
		exit(1);

	new_token->dollar_flag = k;
	new_token->pipe_flag = pipe_flag;
	new_token->next = NULL;

	// Eğer dolar genişletmesi gerekiyorsa expand_variable ile genişlet
	if (k == 1)
	{
		int index = 0;
		expanded_value = expand_variable(add_token, &index);
		if (!expanded_value)
		{
			printf("Error: Memory allocation failed in expand_variable\n");
			free(new_token);
			return;
		}
		free(add_token); // Eski token'ı serbest bırak
		new_token->value = expanded_value; // Yeni genişletilmiş değeri ata
	}
	else
	{
		new_token->value = add_token; // Genişletme yoksa orijinal değeri ata
	}

	// Token listesine ekleme
	if (*token == NULL)
	{
		(*token) = new_token;
	}
	else
	{
		last = *token;
		while (last->next != NULL)
			last = last->next;
		last->next = new_token;
	}
}

/*void	ft_add_token(t_token **token, char *add_token, int k, int pipe_flag)
{
	t_token	*new_token;
	t_token	*last;

	new_token = (t_token*)malloc(sizeof(t_token));
	if (new_token == NULL)
		exit(1);
	new_token->dollar_flag = k;
	new_token->pipe_flag = pipe_flag;
	new_token->value = add_token;
	new_token->next = NULL;
	if (*token == NULL)
	{
		(*token) = new_token;
	}
	else
	{
		last = *token;
		while (last->next != NULL)
			last = last->next;
		last->next = new_token;
	}
}*/

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

int		ft_pipe_flag_check(char *token)
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
}
// ---------------------------------------------------------
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
// yanlış*****
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

char	**ft_pipe_split(char *token)
{
	int	i = 0, j = 0, k, l;
	char **str;
	int pipe_count = ft_pipe_count(token);

	str = (char **)malloc(sizeof(char *) * (2 * pipe_count + 2));
	if (!str)
		return (NULL);

	while (i < (2 * pipe_count + 1))
	{
		k = j;
		ft_where_pipe(token + j, &j);
		l = j - k;

		str[i] = (char *)malloc(sizeof(char) * (l + 1));
		if (!str[i])
		{
			while (i > 0)
				free(str[--i]);
			free(str);
			return (NULL);
		}
		str[i] = ft_strndup(token + k, l);
		printf("-%s\n", str[i]);
		i++;
		if (token[j] == '|')
		{
			str[i] = ft_strdup("|");
			j++; // Pipe'ı atla
			printf("-%s\n", str[i]);
			i++;
		}
	}
	str[i] = NULL;
	return (str);
}
//********
// ------------------------------------------------------------
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

void	ft_pipe_lexer(char *input, t_token **token_list, int quote_flag)
{
	int		i;
	int		start;
	int		j;
	int		k;
	char	*token;
	int		pipe_flag;
	int		pipe_count;
	char	**tok_p;

	i = 0;
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		if (input[i] == '$')
			k = 0;
		if (!input[i])
			break;
		start = i;
		if(dollar_syntax_check(input + i) == 0)
			k = 0;
		if (dollar_syntax_check(input + i) == 1)
			k = 1;
		if (dollar_syntax_check(input + i) == -1)
			k = -1;
		if (input[i] == '"' || input[i] == '\'')
		{
			quote_flag = input[i];
			//i++;
			start = i;
			i++;
			while (input[i] && input[i] != quote_flag)
				i++;
			token = ft_substr(input, start, (i - start) + 1);
			if (!token)
			{
				printf("Error: Memory allocation failed in ft_substr\n");
				return;
			}
			i++;
		}
		else
		{
			while (input[i] && !ft_isspace(input[i]) && input[i] != '"' && input[i] != '\'')
				i++;
			token = ft_substr(input, start, i - start);
			if (!token)
			{
				printf("Error: Memory allocation failed in ft_substr\n");
				return;
			}
		}
		size_t len = ft_strlen(token);
		// printf("%s\n", token);
		// printf("%d\n", ft_all_pipe(token));
		pipe_flag = ft_pipe_flag_check(token);
		//printf("yyyyy%d\n", pipe_flag);
		if (len >= 2 && token[0] == '"' && token[len - 1] == '"' && ft_all_pipe(token))
		{
			char *trimmed = ft_substr(token, 1, len - 2);
			free(token);
			token = trimmed;
			if (!token)
			{
				printf("Error: Memory allocation failed while trimming quotes\n");
				return;
			}
			printf("%s\n", token);
		}
		if (!token_list)
		{
			printf("Error: token_list is NULL before adding a token\n");
			free(token);
			return;
		}
		//printf("eee%d\n", pipe_flag);
		if (pipe_flag == 0)
		{
			if (ft_token_pipe(token))
			{
				pipe_count = ft_pipe_count(token);
				tok_p = ft_pipe_split(token);
				while(pipe_count > j)
				{
					ft_add_token(token_list, tok_p[j], k, pipe_flag);
					j++;	
				}
				print_tokens((*token_list));
				continue;
			}
				
		}
		if (ft_strcmp(token, "") != 0)
			ft_add_token(token_list, token, k, pipe_flag);
	}
	print_tokens((*token_list));	
}

void	ft_lexer(char *input, t_token **token_list)
{
	int		i;
	int		start;
	int		j;
	int		k;
	int		quote_flag = 0;
	int		pipe_flag;
	char	*token;

	i = 0;
	j = 0;
	if (!input || !token_list)
	{
		printf("Error: NULL input or token_list\n");
		return;
	}
	if (ft_strchr(input, '|'))
	{
		ft_pipe_lexer(input, token_list, quote_flag);
		return ;
	}
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i])) 
			i++;
		if (input[i] == '$')
			k = 0;
		if (!input[i])
			break;
		start = i;
		if(dollar_syntax_check(input + i) == 0)
			k = 0;
		if (dollar_syntax_check(input + i) == 1)
			k = 1;
		if (dollar_syntax_check(input + i) == -1)
			k = -1;
		if (input[i] == '"' || input[i] == '\'')
		{
			quote_flag = input[i];
			i++;
			start = i;
			while (input[i] && input[i] != quote_flag)
				i++;
			token = ft_substr(input, start, i - start);
			if (!token)
			{
				printf("Error: Memory allocation failed in ft_substr\n");
				return;
			}
			i++;
		}
		else
		{
			while (input[i] && !ft_isspace(input[i]) && input[i] != '"' && input[i] != '\'')
				i++;
			token = ft_substr(input, start, i - start);
			if (!token)
			{
				printf("Error: Memory allocation failed in ft_substr\n");
				return;
			}
		}
		printf("token test: %s\n", token);
		size_t len = ft_strlen(token);
		if (len >= 2 && token[0] == '"' && token[len - 1] == '"')
		{
			char *trimmed = ft_substr(token, 1, len - 2);
			free(token);
			token = trimmed;
			if (!token)
			{
				printf("Error: Memory allocation failed while trimming quotes\n");
				return;
			}
			printf("%s------\n", token);
		}
		if (!token_list)
		{
			printf("Error: token_list is NULL before adding a token\n");
			free(token);
			return;
		}
		if (ft_strcmp(token, "") != 0)
			ft_add_token(token_list, token, k, pipe_flag);
	}
	print_tokens((*token_list));
}
