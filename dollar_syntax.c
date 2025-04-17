/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bekarada <bekarada@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 13:10:17 by bekarada          #+#    #+#             */
/*   Updated: 2025/03/17 12:08:17 by bekarada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dollar_syntax_check(char *str)
{
	int		count;

	count = 0;
	int i=0;
	//printf("dolar str: %s\n", str);
	while (str[i])
	{
		if (str[i] == '\"')
		{
			i++;
			while (str[i] != '\"')
			{
				if (str[i] == '$')
					return (0);
				i++;
			}			
		}
		if (str[i] == '\'')
		{
			i++;
			while (str[i] != '\'')
			{
				if (str[i] == '$')
					return (1);
				i++;
			}			
		}
		if (str[i] == '$')
		{
			i++;
			if (str[i] != '$' && str[i] != ' ' && !(str[i] >= 9 && str[i] <= 13))
				return (0);
		}
		i++;
	}
	return (-1);
}