/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 05:15:55 by marvinleibe       #+#    #+#             */
/*   Updated: 2024/04/22 02:18:21 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "parser.h"
#include "minishell.h"

// int	execute_unset(t_ast *args)
// {
// 	t_ast	*arg;
// 	char	*var_name;

// 	if (args->right && args->right->type == AST_WHITESPACE)
// 		arg = args->right->right;
// 	else
// 		arg = args->right;
// 	while (arg)
// 	{
// 		if (arg->type == AST_WORD || arg->type == AST_VARIABLE)
// 		{
// 			var_name = arg->data + (arg->type == AST_VARIABLE);
// 			unset_env_var(var_name);
// 		}
// 		arg = arg->right;
// 	}
// 	return (0);
// }
// int	is_valid_variable(char *var)
// {
// 	if (var == NULL || *var == '\0' || strchr(var, '=') != NULL || strchr(var,
// 			'?') != NULL || strchr(var, '$') != NULL)
// 		return (0);
// 	return (1);
// }

int	is_valid_variable(char *var)
{
	char *p;

	p = var + 1;
	if (var == NULL || *var == '\0' || strcmp(var, "\"=\"") == 0 || strcmp(var, "?") == 0 || strcmp(var, "$") == 0)
		return 0;
	if (!isalpha(var[0]) && var[0] != '_')
		return (0);
	while (*p)
	{
		if (!isalnum(*p) && *p != '_')
		{
			return (0);
		}
		p++;
	}
	return (1);
}

int	execute_unset(t_ast *args)
{
	t_ast	*arg;
	char	*var_name;
	int		has_invalid;

	has_invalid = 0;
	arg = args;
	while (arg)
	{
		if ((arg->type == AST_WORD) ||  (arg->type == AST_VARIABLE) || (arg->type == AST_DOUBLEQUOTED_WORD))
		{
			var_name = arg->data + (arg->type == AST_VARIABLE);
			if (is_valid_variable(var_name))
				unset_env_var(var_name);
			else
			{
				ft_fprintf(STDERR_FILENO, "minishell: unset: `%s': not a valid identifier\n",
					var_name);
				has_invalid = 1;
			}
		}
		if (arg->right && arg->right->type == AST_WHITESPACE)
		{
			if (!arg->right->right)
			{
				ft_fprintf(STDERR_FILENO, "minishell: unset: `': not a valid identifier\n");
				has_invalid = 1;
			}
		}
		arg = arg->right;
	}
	if (has_invalid == 1)
		return (1);
	else return (0);
}
