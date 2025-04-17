/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bekarada <bekarada@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 14:40:43 by bekarada          #+#    #+#             */
/*   Updated: 2025/03/22 14:41:52 by bekarada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int n_check(char *str, char c)
{
    int i=0;
    str = str +2;
    while(str[i])
    {
        if(str[i] != c)
        {
            return 1;
        }
        i++;
    }
    return 0;
}
void ft_echo(t_program *prgm)
{
    int newline = 1;
    t_token *token;

    if (!prgm || !prgm->tokens)
        return;

    token = prgm->tokens;
    if (token->value && ft_strcmp(token->value, "echo") == 0)
        token = token->next;
    while (token && token->value && (ft_strncmp(token->value, "-n", 2) == 0))
    {
        if(n_check(token->value,'n'))
        {
            newline = 1;
            break;
        }
        newline = 0;   
        token = token->next;
    }
    while (token)
    {
        if (token->value && token->value[0] != '\0')
        {
            printf("%s", token->value);
            if (token->next && token->next->value && token->next->value[0] != '\0')
                printf(" ");
        }
        token = token->next;
    }
    if (newline)
        printf("\n");
}
