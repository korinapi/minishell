/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 05:16:28 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/04 22:11:17 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	is_only_or_last(t_ast *arg)
{
	if (!arg->right)
		return (1);
	return (0);
}

int	is_option(t_ast *arg)
{
	if ((arg->type == AST_WORD || arg->type == AST_SINGLEQUOTED_WORD
			|| arg->type == AST_DOUBLEQUOTED_WORD) && arg->data[0] == '-'
		&& arg->data[1] && ft_isalpha(arg->data[1]))
			return (1);
	return (0);
}

int	execute_echo(t_ast *args, int *exit_status)
{
	t_ast *arg;
	char *var_name;
	char *exit_stat;
	char *quoted_str;
	int print_newline;

	print_newline = 1;
	exit_stat = ft_itoa(*exit_status);
	if (args->right && args->right->type == AST_WHITESPACE)
		arg = args->right->right;
	else
		arg = args->right;
	if (is_option(arg))
	{
		if (ft_strcmp(arg->data, "-n") == 0)
			print_newline = 0;
		if (arg->right && arg->right->type == AST_WHITESPACE)
			arg = arg->right->right;
		else
			arg = arg->right;
	}
	while (arg)
	{
		if ((arg->type == AST_WORD || arg->type == AST_SINGLEQUOTED_WORD))
			ft_putstr_fd(arg->data, STDOUT_FILENO);
		else if (arg->type == AST_DOUBLEQUOTED_WORD)
		{
			quoted_str = arg->data;
			while (*quoted_str)
			{
				if (*quoted_str == '$' && *(quoted_str + 1) == '?')
				{
					ft_putstr_fd(exit_stat, STDOUT_FILENO);
					quoted_str += 2;
				}
				else
				{
					ft_putchar_fd(*quoted_str, STDOUT_FILENO);
					quoted_str++;
				}
			}
		}
		else if (arg->type == AST_VARIABLE)
		{
			var_name = arg->data + 1;
			ft_putstr_fd(getenv(var_name), STDOUT_FILENO);
		}
		else if (arg->type == AST_SPECIAL_PARAM)
			ft_putstr_fd(exit_stat, STDOUT_FILENO);
		if (arg->right && !is_only_or_last(arg) && arg->type == AST_WHITESPACE)
			ft_putstr_fd(arg->data, STDOUT_FILENO);
		arg = arg->right;
	}
	if (print_newline)
		ft_putendl_fd("", STDOUT_FILENO);
	free(exit_stat);
	return (0);
}