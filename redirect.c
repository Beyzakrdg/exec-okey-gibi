/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bekarada <bekarada@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:52:00 by bekarada          #+#    #+#             */
/*   Updated: 2025/03/16 12:53:23 by bekarada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
#include "minishell.h"

static void toggle_quotes(char current_char, int *single_quotes, int *double_quotes)
{
    if (current_char == '\'' && *double_quotes % 2 == 0)
        (*single_quotes)++;
    if (current_char == '\"' && *single_quotes % 2 == 0)
        (*double_quotes)++;
}

static int is_redirect_op(char c)
{
    return (c == '>' || c == '<');
}

static int validate_redirect_syntax(t_program *program, char *str)
{
    char operator = *str;
    str++;
    if (*str == operator)
        str++;
    while (*str == ' ')
        str++;
    if (is_redirect_op(*str))
        return 1;
    return 0;
}

static char *process_redirects_and_quotes(char *input, int single_quotes, int double_quotes)
{
    int i = 0, j = 0;
    int input_len = ft_strlen(input);
    char *result = malloc(input_len * 3 + 1);
    if (!result)
        return NULL;

    while (input[i])
    {
        toggle_quotes(input[i], &single_quotes, &double_quotes);
        if (is_redirect_op(input[i]) && single_quotes % 2 == 0 && double_quotes % 2 == 0)
        {
            if (j > 0 && result[j - 1] != ' ')
                result[j++] = ' ';
            result[j++] = input[i];
            if (input[i + 1] == input[i])
                result[j++] = input[++i];
            if (input[i + 1] != ' ' && input[i + 1] != '\0')
                result[j++] = ' ';
        }
        else
            result[j++] = input[i];
        i++;
    }
    result[j] = '\0';
    return result;
}

static int detect_redirect_errors(t_program *program, char *str)
{
    if (is_redirect_op(*str))
    {
        if (validate_redirect_syntax(program, str))
        {
            printf("ERROR!! Redirect syntax error!\n");
            return 1;
        }
    }
    return 0;
}

int validate_redirects(t_program *prgm)
{
    int single_quotes = 0;
    int double_quotes = 0;
	char	*line;

	line = prgm->input_program;
    if (line[ft_strlen(line) - 1] == '<' || line[ft_strlen(line) - 1] == '>')
    {
        printf("ERROR!! Redirect syntax error\n");
        return 1;
    }
    prgm->parsed_program = process_redirects_and_quotes(line, single_quotes, double_quotes);
    if (!prgm->parsed_program)
        return 1;
    while (*line)
    {
        toggle_quotes(*line, &single_quotes, &double_quotes);
        if (is_redirect_op(*line) && single_quotes % 2 == 0 && double_quotes % 2 == 0)
        {
            if (detect_redirect_errors(prgm, line))
                return 1;
        }
        line++;
    }
    return 0;
}

*/


#include "minishell.h"

static void toggle_quotes(char current_char, int *single_quotes, int *double_quotes)
{
    if (current_char == '\'' && *double_quotes % 2 == 0)
        (*single_quotes)++;
    if (current_char == '\"' && *single_quotes % 2 == 0)
        (*double_quotes)++;
}

static int is_redirect_op(char c)
{
    return (c == '>' || c == '<');
}

static int validate_redirect_syntax(t_program *program, char *str)
{
    char operator = *str;
    str++;
    if (*str == operator)
        str++;
    while (*str == ' ')
        str++;
    if (is_redirect_op(*str))
        return 1;
    return 0;
}

static char *process_redirects_and_quotes(char *input, int single_quotes, int double_quotes)
{
    int i = 0, j = 0;
    int input_len = ft_strlen(input);
    char *result = malloc(input_len * 3 + 1);
    if (!result)
        return NULL;

    while (input[i])
    {
        toggle_quotes(input[i], &single_quotes, &double_quotes);
        if (is_redirect_op(input[i]) && single_quotes % 2 == 0 && double_quotes % 2 == 0)
        {
            if (j > 0 && result[j - 1] != ' ')
                result[j++] = ' ';
            result[j++] = input[i];
            if (input[i + 1] == input[i])
                result[j++] = input[++i];
            if (input[i + 1] != ' ' && input[i + 1] != '\0')
                result[j++] = ' ';
        }
        else
            result[j++] = input[i];
        i++;
    }
    result[j] = '\0';
    return result;
}

static int detect_redirect_errors(t_program *program, char *str)
{
    if (is_redirect_op(*str))
    {
        if (validate_redirect_syntax(program, str))
        {
            printf("ERROR!! Redirect syntax error!\n");
            return 1;
        }
    }
    return 0;
}

int validate_redirects(t_program *prgm)
{
    int single_quotes = 0;
    int double_quotes = 0;
    char *line;

    line = prgm->input_program;
    if (line[ft_strlen(line) - 1] == '<' || line[ft_strlen(line) - 1] == '>')
    {
        printf("ERROR!! Redirect syntax error\n");
        return 1;
    }
    prgm->parsed_program = process_redirects_and_quotes(line, single_quotes, double_quotes);
    if (!prgm->parsed_program)
        return 1;
    while (*line)
    {
        toggle_quotes(*line, &single_quotes, &double_quotes);
        if (is_redirect_op(*line) && single_quotes % 2 == 0 && double_quotes % 2 == 0)
        {
            if (detect_redirect_errors(prgm, line))
                return 1;
        }
        line++;
    }
    return 0;
}
