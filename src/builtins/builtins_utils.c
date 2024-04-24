/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:03:08 by cpuiu             #+#    #+#             */
/*   Updated: 2024/04/24 03:29:03 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "minishell.h"
#include "parser.h"

void	set_pwd(char ***envp)
{
	char	*pwd;
	char	*cwd;

	pwd = ft_getenv("PWD", *envp);
	if (pwd == NULL || !*pwd)
	{
		cwd = getcwd(NULL, 0);
		if (cwd != NULL)
		{
			if (ft_setenv("PWD", cwd, 1, envp))
				perror("setting pwd error");
			free(cwd);
		}
	}
}


void	process_double_quoted_str(const char *quoted_str, char *exit_stat)
{
	while (*quoted_str)
	{
		if (*quoted_str == '$' && *(quoted_str + 1) == '?')
		{
			ft_putstr_fd(exit_stat, STDOUT_FILENO);
			quoted_str += 2;
		}
		else
		{
			ft_putchar_fd(*quoted_str, STDOUT_FILENO);
			quoted_str++;
		}
	}
}
