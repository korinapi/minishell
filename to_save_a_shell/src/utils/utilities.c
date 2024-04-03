/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 17:32:21 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/03 18:08:32 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

char	**ast_to_arg(t_ast *node)
{
	int		num_arg;
	t_ast	*current;
	char	**args;

	current = node;
	num_arg = 0;
	while (current)
	{
		if (current->type != AST_WHITESPACE)
			num_arg++;
		current = current->right;
	}
	args = ft_calloc(num_arg + 1, sizeof(char *));
	current = node;
	num_arg = 0;
	while (current)
	{
		if (current->type != AST_WHITESPACE)
			args[num_arg++] = ft_strdup(current->data);
		current = current->right;
	}
	args[num_arg] = NULL;
	return (args);
}
