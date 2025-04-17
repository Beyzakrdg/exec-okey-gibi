/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elikavak <elikavak@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:47:11 by bekarada          #+#    #+#             */
/*   Updated: 2025/03/23 15:02:48 by elikavak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int history_check(t_program *prgm)
{
    int i;
    
    i = 0;
    if (!prgm->input_program)
        return (1);
    while (prgm->input_program[i])
    {
        if ((prgm->input_program[i] != 10) && (prgm->input_program[i] != 32))
            return (1);
        i++;
    }
    return (0);
}

char	*ft_strndup(char *str, size_t n)
{
	char	*s;
	size_t	i;

	s = (char *)malloc(n + 1);
	if (!s)
		return (NULL);
	i = 0;
	while (i < n && (str[i] != '\0'))
	{
		s[i] = str[i];
		i++;
	}
	s[i] = '\0';
	return (s);
}

int	ft_isspace(int c)
{
	return (c == '\f' || c == '\n' || c == '\r'
		|| c == '\t' || c == '\v' || c == ' ');
}

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return (unsigned char)(*s1) - (unsigned char)(*s2);
}

char *ft_strcpy(char *dest, const char *src)
{
    int i;

    i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

#include <stddef.h>

char *ft_strcat(char *dest, const char *src)
{
    int i = 0;
    int j = 0;

    while (dest[i] != '\0')
    {
        i++;
    }
    while (src[j] != '\0')
    {
        dest[i] = src[j];
        i++;
        j++;
    }
    dest[i] = '\0';

    return dest;
}
