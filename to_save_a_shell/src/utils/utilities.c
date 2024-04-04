/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 17:32:21 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/04 22:33:14 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

char	**ast_to_arg(t_ast *node, int *exit_status)
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
		{
			if (current->type == AST_SPECIAL_PARAM)
				args[num_arg++] = ft_itoa(*exit_status);
			else
				args[num_arg++] = ft_strdup(current->data);
		}
		current = current->right;
	}
	args[num_arg] = NULL;
	return (args);
}

int	is_valid(const char *str)
{
	if (!*str)
		return (0);
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	str++;
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	if (ft_strcmp(str, "for") == 0 || ft_strcmp(str, "while") == 0 || ft_strcmp(str,
			"if") == 0 || ft_strcmp(str, "case") == 0)
		return (0);
	return (1);
}
