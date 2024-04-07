/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 05:16:03 by marvinleibe       #+#    #+#             */
/*   Updated: 2024/04/07 04:30:51 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "minishell.h"
#include "parser.h"
#include "utilities.h"

int	compare_strings(const char *a, const char *b)
{
	return (ft_strcmp(a, b));
}

void	swap_strings(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	bubble_sort(int n)
{
	int	i;
	int	j;

	i = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - i - 1)
		{
			if (compare_strings(environ[j], environ[j + 1]) > 0)
				swap_strings(&environ[j], &environ[j + 1]);
			j++;
		}
		i++;
	}
}

int	valid_check(t_ast *arg, char *var_name)
{
	if (!is_valid(var_name))
	{
		ft_putstr_fd("export: `", STDERR_FILENO);
		ft_putstr_fd(arg->data, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	execute_export(t_ast *args)
{
	t_ast	*arg;
	char	*value;
	char	*var_name;
	int		i;
	int		env_count;

	i = 0;
	arg = NULL;
	if (args->right && args->right->type == AST_WHITESPACE
		&& args->right->right)
		arg = args->right->right;
	else if (args->right && args->right->type != AST_WHITESPACE)
		arg = args->right;
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
		return (0);
	}
	while (arg)
	{
		if (arg->type == AST_WORD)
		{
			value = ft_strchr(arg->data, '=');
			if (value)
			{
				*value = '\0';
				value++;
				var_name = arg->data;
				if (valid_check(arg, var_name))
					return (1);
				ft_setenv(var_name, value, 1);
			}
			else
			{
				var_name = arg->data;
				if (valid_check(arg, var_name))
					return (1);
				ft_setenv(var_name, "", 0);
			}
		}
		arg = arg->right;
	}
	return (0);
}
