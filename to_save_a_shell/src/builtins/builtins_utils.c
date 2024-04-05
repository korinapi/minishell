/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:05:21 by cpuiu             #+#    #+#             */
/*   Updated: 2024/04/05 14:06:03 by cpuiu            ###   ########.fr       */
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
