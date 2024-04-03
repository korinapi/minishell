/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 05:30:21 by marvinleibe       #+#    #+#             */
/*   Updated: 2024/04/01 22:31:55 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "minishell.h"
#include "parser.h"

int	execute_cd(t_ast *args)
{
	t_ast *arg;
	char *path;
	char *var_name;
	char cwd[PATH_MAX];

	var_name = NULL;
	if (args->right && args->right->type == AST_WHITESPACE)
		arg = args->right->right;
	else
		arg = args->right;
	if (!arg || arg->type == AST_WHITESPACE || !ft_strcmp(arg->data, "~"))
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
			return (1);
		}
	}
	else if (!ft_strcmp(arg->data, "-") && !*(arg->data + 1))
	{
		path = getenv("OLDPWD");
		if (!path)
		{
			ft_putendl_fd("cd: OLDPWD not set", STDERR_FILENO);
			return (1);
		}
	}
	else if (arg->type == AST_WORD || arg->type == AST_VARIABLE)
	{
		if (arg->type == AST_VARIABLE)
		{
			var_name = (arg->data + 1);
			path = getenv(var_name);
		}
		else
			path = arg->data;
	}
	else
	{
		ft_putendl_fd("cd: invalid argument", STDERR_FILENO);
		return (1);
	}
	if (chdir(path) != 0)
	{
		ft_putendl_fd("errno", STDERR_FILENO);
		return (1);
	}
	ft_setenv("OLDPWD", getenv("PWD"), 1);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_setenv("PWD", cwd, 1);
	else
	{
		ft_putendl_fd("cd: getcwd error", STDERR_FILENO);
		return (1);
	}
	return (0);
}