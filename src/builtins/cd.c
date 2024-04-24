/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 05:30:21 by marvinleibe       #+#    #+#             */
/*   Updated: 2024/04/24 03:29:51 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "environment.h"
#include "minishell.h"
#include "parser.h"

int	handle_home(t_ast *arg, char **path, char ***envp)
{
	if (!arg || arg->type == AST_WHITESPACE || !ft_strcmp(arg->data, "~"))
	{
		*path = ft_getenv("HOME", *envp);
		if (!*path)
		{
			ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
			return (1);
		}
		return (0);
	}
	return (0);
}

int	handle_old(t_ast *arg, char **path, char ***envp)
{
	if (arg && !ft_strcmp(arg->data, "-") && !*(arg->data + 1))
	{
		*path = ft_getenv("OLDPWD", *envp);
		if (!*path)
		{
			ft_putendl_fd("cd: OLDPWD not set", STDERR_FILENO);
			return (1);
		}
		return (0);
	}
	return (0);
}

int	handle_var(t_ast *arg, char **path, char ***envp)
{
	char	*var_name;

	if (arg && (arg->type == AST_WORD || arg->type == AST_VARIABLE
			|| arg->type == AST_DOUBLEQUOTED_WORD
			|| arg->type == AST_SINGLEQUOTED_WORD))
	{
		if (arg->type == AST_VARIABLE)
		{
			var_name = (arg->data + 1);
			if (!ft_strcmp(var_name, "PWD"))
			{
				if (!*path)
					*path = ft_getenv("PWD", *envp);
				return (0);
			}
			if (!*path)
				*path = ft_getenv(var_name, *envp);
		}
		else if (!*path)
			*path = arg->data;
		return (0);
	}
	return (0);
}

int	handle_pwd(char *cwd, char ***envp)
{
	if (getcwd(cwd, PATH_MAX) != NULL)
	{
		ft_setenv("OLDPWD", ft_getenv("PWD", *envp), 1, envp);
		ft_setenv("PWD", cwd, 1, envp);
		return (0);
	}
	else
	{
		ft_putendl_fd("cd: getcwd error", STDERR_FILENO);
		return (1);
	}
}

int	execute_cd(t_ast *args, char ***envp)
{
	t_ast	*arg;
	char	*path;
	char	cwd[PATH_MAX];

	path = NULL;
	set_pwd(envp);
	if (args->right && args->right->type == AST_WHITESPACE)
		arg = args->right->right;
	else
		arg = args->right;
	if (handle_home(arg, &path, envp))
		return (1);
	if (handle_old(arg, &path, envp))
		return (1);
	if (handle_var(arg, &path, envp))
		return (1);
	if (chdir(path) != 0)
	{
		ft_putendl_fd(" No such file or directory", STDERR_FILENO);
		return (1);
	}
	if (handle_pwd(cwd, envp))
		return (1);
	return (0);
}
