/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bekarada <bekarada@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 14:39:43 by bekarada          #+#    #+#             */
/*   Updated: 2025/03/22 14:39:57 by bekarada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_program *prgm)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd != NULL)
	{
		printf("%s\n", pwd);
		free(pwd);
	}
	else
		perror("getcwd");
}
