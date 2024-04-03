/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 05:16:03 by marvinleibe       #+#    #+#             */
/*   Updated: 2024/04/01 22:32:01 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"

int	compare_strings(const char *a, const char *b)
{
	return (strcmp(a, b));
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
	int i, j;
	for (i = 0; i < n - 1; i++)
	{
		for (j = 0; j < n - i - 1; j++)
		{
			if (compare_strings(environ[j], environ[j + 1]) > 0)
			{
				swap_strings(&environ[j], &environ[j + 1]);
			}
		}
	}
}

int	execute_export(t_ast *args)
{
	t_ast *arg;
	char *value;
	char *var_name;
	int i;
	int env_count;

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
		for (i = 0; i < env_count; i++)
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putendl_fd(environ[i], STDOUT_FILENO);
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
				ft_setenv(var_name, value, 1);
			}
			else
			{
				var_name = arg->data;
				ft_setenv(var_name, "", 0);
			}
		}
		arg = arg->right;
	}

	return (0);
}