/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_extra.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 20:43:53 by cpuiu             #+#    #+#             */
/*   Updated: 2024/04/29 17:22:21 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "environment.h"
#include "errors.h"
#include "executor.h"
#include "free.h"
#include "minishell.h"
#include "redirection.h"
#include "utilities.h"

void	close_pipes(int *pipe_fds, int num_pipes)
{
	int	i;

	i = 0;
	while (i < 2 * num_pipes)
	{
		close(pipe_fds[i]);
		i++;
	}
}

void	fill_args(t_ast *current_node, int *exit_status, char **args)
{
	int	i;

	i = 0;
	while (current_node)
	{
		if (current_node->type != AST_WHITESPACE
			&& current_node->type != AST_REDIRECTION)
		{
			if (current_node->type != AST_SINGLEQUOTED_WORD
				&& !ft_strcmp(current_node->data, "$?"))
			{
				free(current_node->data);
				current_node->data = ft_itoa(*exit_status);
			}
			args[i++] = current_node->data;
		}
		current_node = current_node->right;
	}
	args[i] = NULL;
}

int	syntax_check_extra(t_ast *ast, int *exit_status)
{
	static char	*symbol;

	if ((ast->left->redirection_mode == REDIR_IN
			|| ast->left->redirection_mode == REDIR_OUT
			|| ast->left->redirection_mode == REDIR_OUT_APPEND
			|| ast->left->redirection_mode == REDIR_HEREDOC))
	{
		if (ft_strcmp(ast->left->redirection_file, "\0") == 0)
		{
			symbol = "newline";
			ft_fprintf(STDERR_FILENO,
				"minishell: syntax error near unexpected token `%s'\n", symbol);
			*exit_status = 258;
			return (1);
		}
	}
	return (0);
}

int	syntax_check_extra_2(t_ast *ast, int *exit_status)
{
	if (ast->type == AST_PIPELINE && (ast->left->type == AST_SIMPLE_COMMAND)
		&& (ast->left->left && ast->left->left->type == AST_REDIRECTION
			&& (ast->left->left->redirection_mode == REDIR_IN
				|| ast->left->left->redirection_mode == REDIR_HEREDOC)
			&& ft_strcmp(ast->left->left->redirection_file, "\0") == 0))
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n");
		*exit_status = 258;
		return (1);
	}
	return (0);
}

int	syntax_check(t_ast *ast, int *exit_status)
{
	if (!ast->left)
	{
		*exit_status = 127;
		return (1);
	}
	if (ast->type == AST_PIPELINE && !ast->left->left)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n");
		*exit_status = 258;
		return (1);
	}
	if (syntax_check_extra_2(ast, exit_status) == 1)
		return (1);
	if (ast->type == AST_SIMPLE_COMMAND)
	{
		if (syntax_check_extra(ast, exit_status) == 1)
			return (1);
	}
	return (0);
}
