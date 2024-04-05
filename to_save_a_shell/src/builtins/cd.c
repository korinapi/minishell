/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 05:30:21 by marvinleibe       #+#    #+#             */
/*   Updated: 2024/04/05 13:14:24 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "minishell.h"
#include "parser.h"

int	put_message_cd(char *str)
{
	ft_putendl_fd(str, STDERR_FILENO);
	return (1);
}

int	get_path_or_variable_value(t_ast *arg, char **path)
{
	char	*var_name;

	if (arg->type == AST_VARIABLE)
	{
		var_name = (arg->data + 1);
		if (!ft_strcmp(var_name, "PWD"))
			return (0);
		*path = getenv(var_name);
	}
	else
		*path = arg->data;
	return (1);
}

int	process_cd_arg(t_ast *arg, char **path)
{
	int	flag;

	flag = 0;
	if (!arg || arg->type == AST_WHITESPACE || !ft_strcmp(arg->data, "~"))
	{
		*path = getenv("HOME");
		if (!*path)
			flag = put_message_cd("cd: HOME not set");
	}
	else if (!ft_strcmp(arg->data, "-") && !*(arg->data + 1))
	{
		*path = getenv("OLDPWD");
		if (!*path)
			flag = put_message_cd("cd: OLDPWD not set");
	}
	else if (arg->type == AST_WORD || arg->type == AST_VARIABLE)
		flag = get_path_or_variable_value(arg, path);
	else
		flag = put_message_cd("cd: invalid argument");
	return (flag);
}

int	execute_cd(t_ast *args)
{
	t_ast	*arg;
	char	*path;
	char	*var_name;
	char	cwd[PATH_MAX];
	int		flag;

	flag = 0;
	var_name = NULL;
	path = NULL;
	if (args->right && args->right->type == AST_WHITESPACE)
		arg = args->right->right;
	else
		arg = args->right;
	flag = process_cd_arg(arg, &path);
	if (chdir(path) != 0)
		flag = put_message_cd("No such file or directory");
	ft_setenv("OLDPWD", getenv("PWD"), 1);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_setenv("PWD", cwd, 1);
	else
		flag = put_message_cd("cd: getcwd error");
	if (flag == 1)
		return (1);
	return (0);
}
