/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 05:15:55 by marvinleibe       #+#    #+#             */
/*   Updated: 2024/04/24 03:23:26 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "minishell.h"
#include "parser.h"

int	is_valid_variable(char *var)
{
	char	*p;

	p = var + 1;
	if (var == NULL || *var == '\0' || ft_strcmp(var, "\"=\"") == 0
		|| ft_strcmp(var, "?") == 0 || ft_strcmp(var, "$") == 0)
		return (0);
	if (!ft_isalpha(var[0]) && var[0] != '_')
		return (0);
	while (*p)
	{
		if (!ft_isalnum(*p) && *p != '_')
			return (0);
		p++;
	}
	return (1);
}

void	conditionals(t_ast *arg, int *has_invalid, char ***envp)
{
	char	*var_name;

	if ((arg->type == AST_WORD) || (arg->type == AST_VARIABLE)
		|| (arg->type == AST_DOUBLEQUOTED_WORD)
		|| arg->type == AST_SINGLEQUOTED_WORD)
	{
		var_name = arg->data + (arg->type == AST_VARIABLE);
		if (is_valid_variable(var_name))
			unset_env_var(var_name, envp);
		else
		{
			ft_fprintf(STDERR_FILENO,
				"minishell: unset: `%s': not a valid identifier\n", var_name);
			*has_invalid = 1;
		}
	}
}

int	execute_unset(t_ast *args, char ***envp)
{
	t_ast	*arg;
	int		has_invalid;

	has_invalid = 0;
	arg = args;
	while (arg)
	{
		conditionals(arg, &has_invalid, envp);
		if (arg->right && arg->right->type == AST_WHITESPACE)
		{
			if (!arg->right->right)
			{
				ft_fprintf(STDERR_FILENO,
					"minishell: unset: `': not a valid identifier\n");
				has_invalid = 1;
			}
		}
		arg = arg->right;
	}
	if (has_invalid == 1)
		return (1);
	else
		return (0);
}
