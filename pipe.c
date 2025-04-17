/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bekarada <bekarada@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:49:05 by bekarada          #+#    #+#             */
/*   Updated: 2025/03/17 15:03:05 by bekarada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void pipe_handle_quotes_check(char temp, int *squotes, int *dquotes)
{
    if (temp == 34 && *squotes % 2 == 0) 
        (*dquotes)++;
    if (temp == 39 && *dquotes % 2 == 0)
        (*squotes)++;
}

int pipe_check(char *line, int sq, int dq)
{
    int i = 0;
    while (line[i] == '\"' || line[i] == '\'')
        i++;
    if (line[i] == '|')
    {
        printf("Error: Pipe syntax error\n");
        return 1;
    }
    i = ft_strlen(line) - 1;
    while (line[i] == '\"' || line[i] == '\'')
        i--;
    if (line[ft_strlen(line) - 1] == '|')
    {
        printf("Error: Pipe syntax error\n");
        return 1;
    }
    while (*line)
    {
        pipe_handle_quotes_check(*line, &sq, &dq);
        if ((*line == '|' || *line == '<' || *line == '>') && sq % 2 == 0
            && dq % 2 == 0)
        {
            line++;
            if (*line == '<' || *line == '>')
                line++;
            while (*line == ' ')
                line++;
            if (*line == '|')
            {
                if (*(line + 1) == '|')
                {
                     printf("Error: Pipe syntax error\n");
                   return 1;
                }
            }
        }
        if (*line)
            line++;
    }
    return 0;
}
