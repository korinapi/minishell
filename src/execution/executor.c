/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:29:28 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/26 15:24:56 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "environment.h"
#include "errors.h"
#include "executor.h"
#include "free.h"
#include "minishell.h"
#include "redirection.h"
#include "signals.h"
#include "utilities.h"

int	execute_external(char **args, int *exit_status, char ***envp)
{
	pid_t	pid;
	char	*command_path;

	setup_child_signals();
	rl_catch_signals = 1;
	command_path = get_command_path(args[0], *envp);
	if (!command_path)
	{
		ft_error("minishell", "command not found", args[0]);
		*exit_status = 127;
		return (free(command_path), *exit_status);
	}
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	else if (pid == 0)
	{
		execute_command_from_path(args, command_path, exit_status, *envp);
		return (1);
	}
	else
		*exit_status = wait_and_update_status(pid);
	rl_catch_signals = 0;
	return (free(command_path), *exit_status);
}

void	execute_simple_command(t_ast *node, int *exit_status, char ***envp)
{
	char	**args;
	t_ast	*current_node;
	t_ast	*root;

	root = node;
	current_node = node->left;
	if (handle_redirection(current_node, exit_status))
		return ;
	args = ft_calloc(ast_count_nodes(current_node) + 1, sizeof(char *));
	if (!args)
		return ;
	fill_args(current_node, exit_status, args);
	if (is_builtin(args[0]))
	{
		free(args);
		*exit_status = execute_builtin(node, root, exit_status, envp);
		return ;
	}
	else
	{
		if (*args)
			*exit_status = execute_external(args, exit_status, envp);
	}
	setup_signals();
	free(args);
}

void	execute_ast(t_ast *ast, int *exit_status, char ***envp)
{
	int	saved_stdin;
	int	saved_stdout;

	if (syntax_check(ast, exit_status))
		return ;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	execute_pipeline(ast, exit_status, envp);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return ;
}

int	execute_external_without_forks(char **args, int *exit_status, char ***envp)
{
	char	*command_path;

	setup_child_signals();
	rl_catch_signals = 1;
	if (!*args)
		return (0);
	command_path = get_command_path(args[0], *envp);
	if (!command_path)
	{
		ft_error("minishell", "command not found", args[0]);
		*exit_status = 127;
		return (free(command_path), *exit_status);
	}
	execute_command_from_path(args, command_path, exit_status, *envp);
	return (1);
	rl_catch_signals = 0;
	return (free(command_path), *exit_status);
}

void	execute_simple_command_without_forks(t_ast *node, int *exit_status,
		char ***envp)
{
	char	**args;
	t_ast	*current_node;
	t_ast	*root;

	root = node;
	current_node = node->left;
	if (handle_redirection(current_node, exit_status))
		return ;
	args = ft_calloc(ast_count_nodes(current_node) + 1, sizeof(char *));
	if (!args)
		return ;
	fill_args(current_node, exit_status, args);
	if (is_builtin(args[0]))
	{
		free(args);
		*exit_status = execute_builtin(node, root, exit_status, envp);
		exit(*exit_status);
	}
	else
		*exit_status = execute_external_without_forks(args, exit_status, envp);
	setup_signals();
	free(args);
}
