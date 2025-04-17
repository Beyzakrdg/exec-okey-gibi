/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bekarada <bekarada@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:48:05 by bekarada          #+#    #+#             */
/*   Updated: 2025/03/16 12:48:43 by bekarada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_single_quote(t_program *prgm, int *i)
{
	int j = 0;

	(*i)++;
	if (prgm->input_program[*i] == '\'')
		j = 1;
	while (prgm->input_program[*i] != '\'' && prgm->input_program[*i] != '\0')
	{
		if (prgm->input_program[*i + 1] == '\'')
			j++;
		(*i)++;
	}
	return (j);
}

static int	handle_double_quote(t_program *prgm, int *i)
{
	int j = 0;

	(*i)++;
	if (prgm->input_program[*i] == '\"')
		j = 1;
	while (prgm->input_program[*i] != '\"' && prgm->input_program[*i] != '\0')
	{
		if (prgm->input_program[*i + 1] == '\"')
			j++;
		(*i)++;
	}
	return (j);
}

int	handle_quotes(t_program *prgm)
{
	int	i;
	int	j;

	i = 0;
	while (prgm->input_program[i] != '\0')
	{
		if (prgm->input_program[i] == '\"')
		{
			j = handle_double_quote(prgm, &i);
			if (j == 0)
			{
				printf("Syntax Error\n");
				return (1);
			}
		}
		else if (prgm->input_program[i] == '\'')
		{
			j = handle_single_quote(prgm, &i);
			if (j == 0)
			{
				printf("Syntax Error\n");
				return (1);
			}
		}
		i++;
	}
	return (0);
}

/*int handle_quotes(t_program *prgm)
{
    int i= 0;
    int j = 0;
    while(prgm->input_program[i] != '\0')
    {
        if (prgm->input_program[i] == '\"')
        {
            i++;
			if (prgm->input_program[i] == '\"')
				j = 1;
            while(prgm->input_program[i] != '\"' && prgm->input_program[i] != '\0')
            {
				if (prgm->input_program[i + 1] == '\"')
					j++;
                i++;
            }
			if (j == 0)
			{
				printf("Sytnax Error\n");
				return 1;
			}
			j = 0;
        }
        if (prgm->input_program[i] == '\'')
        {
            j=0;
            i++;
			if (prgm->input_program[i] == '\'')
				j = 1;
            while(prgm->input_program[i] != '\'' && prgm->input_program[i] != '\0')
            {
				if (prgm->input_program[i + 1] == '\'')
					j++;
                i++;
            }
			if (j == 0)
			{
				printf("Sytnax Error\n");
				return 1;
			}
        }
        i++;
    }
    return 0;
}
*/