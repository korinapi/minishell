/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:29:28 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/05 20:40:14 by cpuiu            ###   ########.fr       */
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
		if (current_node->type != AST_WHITESPACE)
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
	return (0);
}

void	execute_simple_command(t_ast *node, int *exit_status)
{
	if (is_builtin(node->left->data))
		*exit_status = execute_builtin(node, exit_status);
	else
		*exit_status = execute_external(node, exit_status);
}

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

int	execute_ast(t_ast *ast, int *exit_status)
{
	int	saved_stdin;
	int	saved_stdout;

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
		return (0);
	}
	else if (ast->type == AST_PIPELINE)
	{
		execute_pipeline(ast, exit_status);
		return (0);
	}
	return (1);
}
