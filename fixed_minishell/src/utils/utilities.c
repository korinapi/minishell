/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 17:32:21 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/12 17:21:08 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

int	count_nodes(t_ast *current)
{
	int	num_arg;

	num_arg = 0;
	while (current)
	{
		if (current->type != AST_WHITESPACE)
			num_arg++;
		current = current->right;
	}
	return (num_arg);
}

int	ast_count_nodes(t_ast *ast)
{
	int	count;

	count = 1;
	if (ast == NULL)
		return (0);
	count += ast_count_nodes(ast->left);
	count += ast_count_nodes(ast->right);
	return (count);
}

char	**ast_to_arg(t_ast *node, int *exit_status)
{
	int		num_arg;
	t_ast	*current;
	char	**args;

	current = node;
	num_arg = count_nodes(current);
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
	if (!ft_isalpha(*str) && *str != '_' && *str != '/')
		return (0);
	str++;
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_' && *str != '/')
			return (0);
		str++;
	}
	if (ft_strcmp(str, "for") == 0 || ft_strcmp(str, "while") == 0
		|| ft_strcmp(str, "if") == 0 || ft_strcmp(str, "case") == 0)
		return (0);
	return (1);
}

int	is_builtin(char *args)
{
	if (args == NULL)
		return (0);
	if (!ft_strcmp(args, "echo"))
		return (1);
	if (!ft_strcmp(args, "cd"))
		return (1);
	if (!ft_strcmp(args, "pwd"))
		return (1);
	if (!ft_strcmp(args, "unset"))
		return (1);
	if (!ft_strcmp(args, "export"))
		return (1);
	if (!ft_strcmp(args, "env"))
		return (1);
	if (!ft_strcmp(args, "exit"))
		return (1);
	return (0);
}
