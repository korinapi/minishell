/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 05:16:28 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/05 12:35:52 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	is_option(t_ast *arg)
{
	if ((arg->type == AST_WORD || arg->type == AST_SINGLEQUOTED_WORD
			|| arg->type == AST_DOUBLEQUOTED_WORD) && arg->data[0] == '-'
		&& arg->data[1] && ft_isalpha(arg->data[1]))
	{
		return (1);
	}
	return (0);
}

void	check_flag_echo(t_ast **arg, int *print_newline)
{
	const char	*options;
	int			contains_n_only;

	options = (*arg)->data + 1;
	contains_n_only = 1;
	while (*options)
	{
		if (*options != 'n')
		{
			contains_n_only = 0;
			break ;
		}
		options++;
	}
	if (contains_n_only)
	{
		print_newline = 0;
		if ((*arg)->right && (*arg)->right->type == AST_WHITESPACE)
			(*arg) = (*arg)->right->right;
		else
			(*arg) = (*arg)->right;
	}
}

void	process_double_quoted_str(const char *quoted_str, const char *exit_stat)
{
	while (*quoted_str)
	{
		if (*quoted_str == '$' && *(quoted_str + 1) == '?')
		{
			printf("%s", exit_stat);
			quoted_str += 2;
		}
		else
		{
			ft_putchar_fd(*quoted_str, STDOUT_FILENO);
			quoted_str++;
		}
	}
}

void	print_ast_arguments(t_ast *arg, char *exit_stat)
{
	char	*var_name;

	if ((arg->type == AST_WORD || arg->type == AST_SINGLEQUOTED_WORD))
		ft_putstr_fd(arg->data, STDOUT_FILENO);
	else if (arg->type == AST_DOUBLEQUOTED_WORD)
		process_double_quoted_str(arg->data, exit_stat);
	else if (arg->type == AST_VARIABLE)
	{
		var_name = arg->data + 1;
		ft_putstr_fd(getenv(var_name), STDOUT_FILENO);
	}
	else if (arg->type == AST_SPECIAL_PARAM)
		ft_putstr_fd(exit_stat, STDOUT_FILENO);
	if (arg->right && arg->right && arg->type == AST_WHITESPACE)
		ft_putstr_fd(arg->data, STDOUT_FILENO);
}

int	execute_echo(t_ast *args, int *exit_status)
{
	t_ast	*arg;
	char	*exit_stat;
	int		print_newline;

	print_newline = 1;
	exit_stat = ft_itoa(*exit_status);
	if (args->right && args->right->type == AST_WHITESPACE)
		arg = args->right->right;
	else
		arg = args->right;
	if (is_option(arg))
		check_flag_echo(&arg, &print_newline);
	while (arg)
	{
		print_ast_arguments(arg, exit_stat);
		arg = arg->right;
	}
	if (print_newline)
		ft_putendl_fd("", STDOUT_FILENO);
	free(exit_stat);
	return (0);
}
