/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 05:16:03 by marvinleibe       #+#    #+#             */
/*   Updated: 2024/04/19 10:17:23 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "environment.h"
#include "minishell.h"
#include "parser.h"
#include "utilities.h"

int	set_environment_variable(t_ast *arg, char *value)
{
	char	*var_name;

	var_name = arg->data;
	if (valid_check(arg, var_name))
		return (1);
	if (!*value && arg->right && arg->right->type == AST_VARIABLE)
	{
		value = getenv(arg->right->data + 1);
		if (value)
			value++;
		else
			return (1);
	}
	ft_setenv(var_name, value, 1);
	return (0);
}

int	process_export_arg(t_ast *arg)
{
	char	*value;
	char	*var_name;
	int		result;

	if (arg->type == AST_WORD)
	{
		value = ft_strchr(arg->data, '=');
		if (value)
		{
			*value = '\0';
			value++;
			result = set_environment_variable(arg, value);
			if (result)
				return (result);
		}
		else
		{
			var_name = arg->data;
			if (valid_check(arg, var_name))
				return (1);
			ft_setenv(var_name, "", 0);
		}
	}
	return (0);
}

t_ast	*find_arg_or_print_env(t_ast *args)
{
	int	env_count;
	int	i;

	i = 0;
	if (args->right && args->right->type == AST_WHITESPACE
		&& args->right->right)
		return (args->right->right);
	else if (args->right && args->right->type != AST_WHITESPACE)
		return (args->right);
	else
	{
		env_count = 0;
		while (environ[env_count])
			env_count++;
		bubble_sort(env_count);
		while (i < env_count)
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putendl_fd(environ[i], STDOUT_FILENO);
			i++;
		}
		return (NULL);
	}
}

int	execute_export(t_ast *args)
{
	t_ast	*arg;

	arg = find_arg_or_print_env(args);
	if (!arg)
		return (0);
	while (arg)
	{
		if (process_export_arg(arg) != 0)
			return (1);
		arg = arg->right;
	}
	return (0);
}