/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 05:15:55 by marvinleibe       #+#    #+#             */
/*   Updated: 2024/04/03 17:53:34 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "parser.h"

int	execute_unset(t_ast *args)
{
	t_ast *arg;
	char *var_name;

	if (args->right && args->right->type == AST_WHITESPACE)
		arg = args->right->right;
	else
		arg = args->right;
	while (arg)
	{
		if (arg->type == AST_WORD || arg->type == AST_VARIABLE)
		{
			var_name = arg->data + (arg->type == AST_VARIABLE);
			unset_env_var(var_name);
		}
		arg = arg->right;
	}
	return (0);
}