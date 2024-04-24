/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 05:16:03 by marvinleibe       #+#    #+#             */
/*   Updated: 2024/04/24 03:24:13 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "environment.h"
#include "minishell.h"
#include "parser.h"
#include "utilities.h"

int	set_environment_variable(t_ast *arg, char *value, char ***envp)
{
	char	*var_name;

	var_name = arg->data;
	if (valid_check(arg, var_name))
		return (1);
	if (!*value && arg->right && arg->right->type == AST_VARIABLE)
	{
		value = ft_getenv(arg->right->data + 1, *envp);
		if (value)
			value++;
		else
			return (1);
	}
	ft_setenv(var_name, value, 1, envp);
	return (0);
}

int	process_export_arg(t_ast *arg, char ***envp)
{
	char	*value;
	char	*var_name;
	int		result;

	if (arg->type == AST_WORD || arg->type == AST_DOUBLEQUOTED_WORD)
	{
		value = ft_strchr(arg->data, '=');
		if (value)
		{
			*value = '\0';
			value++;
			result = set_environment_variable(arg, value, envp);
			if (result)
				return (result);
		}
		else
		{
			var_name = arg->data;
			if (valid_check(arg, var_name))
				return (1);
			ft_setenv(var_name, "", 0, envp);
		}
	}
	return (0);
}

t_ast	*find_arg_or_print_env(t_ast *args, char **envp)
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
		while (envp[env_count])
			env_count++;
		bubble_sort(envp, env_count);
		while (i < env_count)
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putendl_fd(envp[i], STDOUT_FILENO);
			i++;
		}
		return (NULL);
	}
}

int	execute_export(t_ast *args, char ***envp)
{
	t_ast	*arg;

	arg = find_arg_or_print_env(args, *envp);
	if (!arg)
		return (0);
	while (arg)
	{
		if (process_export_arg(arg, envp) != 0)
			return (1);
		arg = arg->right;
	}
	return (0);
}
