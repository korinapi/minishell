/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:29:28 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/07 23:40:33 by mleibeng         ###   ########.fr       */
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

int	execute_external(t_ast *ast, int *exit_status)
{
	pid_t	pid;
	char	**args;
	int		i;
	t_ast	*current_node;

	args = ft_calloc(ast_count_nodes(ast->left) + 1, sizeof(char *));
	i = 0;
	current_node = ast->left;
	while (current_node)
	{
		if (current_node->type != AST_WHITESPACE && current_node->type != AST_REDIRECTION)
			args[i++] = current_node->data;
		current_node = current_node->right;
	}
	args[i] = NULL;
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	else if (pid == 0)
		execute_command_from_path(args);
	else
		wait_and_update_status(pid, exit_status);
	free(args);
	return (*exit_status);
}

void	execute_simple_command(t_ast *node, int *exit_status)
{
	if (is_builtin(node->left->data))
		*exit_status = execute_builtin(node, exit_status);
	else
		*exit_status = execute_external(node, exit_status);
}

int	syntax_check(t_ast *ast, int *exit_status)
{
	if (ast->type == AST_PIPELINE && !ast->left->left)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n");
		*exit_status = 258;
		return (1);
	}
	return (0);
}

void	execute_ast(t_ast *ast, int *exit_status)
{
	int	saved_stdin;
	int	saved_stdout;

	if (syntax_check(ast, exit_status))
		return ;
	if (ast->type == AST_SIMPLE_COMMAND)
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		handle_redirection(ast);
		execute_simple_command(ast, exit_status);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return ;
	}
	else if (ast->type == AST_PIPELINE)
	{
		execute_pipeline(ast, exit_status);
		return ;
	}
	return ;
}
