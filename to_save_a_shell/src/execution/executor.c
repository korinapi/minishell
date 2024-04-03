/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:29:28 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/03 13:59:27 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "environment.h"
#include "errors.h"
#include "executor.h"
#include "free.h"
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

char	*find_command_path(char *command, char **paths)
{
	char	*full_path;
	int		i;

	if (ft_strchr(command, '/') != NULL)
	{
		if (!access(command, X_OK))
			return (ft_strdup(command));
		else
			return (NULL);
	}
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin_free(full_path, command, 1);
		if (!access(full_path, X_OK))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

// ft_free_split would be necessary here if I didnt already free the ast elements later on...
// Since i do not strdup the data in the nodes, I dont need to free.
int	execute_external(t_ast *ast, int *exit_status)
{
	pid_t	pid;
	int		status;
	char	**args;
	int		i;
	char	*path_env;
	char	**paths;
	char	*command_path;
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
	{
		perror("fork");
		return (1);
	}
	else if (pid == 0)
	{
		path_env = getenv("PATH");
		paths = ft_split(path_env, ':');
		command_path = find_command_path(args[0], paths);
		if (command_path == NULL)
		{
			ft_putstr_fd("Command not found: ", STDERR_FILENO);
			ft_putstr_fd(args[0], STDERR_FILENO);
			ft_putchar_fd('\n', STDERR_FILENO);
			exit(127);
		}
		execve(command_path, args, NULL);
		perror("execve");
		exit(1);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			*exit_status = WEXITSTATUS(status);
		else
			*exit_status = 1;
	}
	return (0);
}

static void	execute_simple_command(t_ast *node, int *exit_status)
{
	if (is_builtin(node->left->data))
		*exit_status = execute_builtin(node, exit_status);
	else
		*exit_status = execute_external(node, exit_status);
}

//Since I fork both in execute_pipeline and in execute_external
//The issue that arises is that I get too many child processes working independently
//that feed their input into a new readline once the cycle is not finished.
//My child process therefore become its own shell trying to build its own tree
//and then execute that tree so the result becomes a command which definitely doesnt work.
//Then the process finishes and my main process quits.
//Solution: Either stop double forks (need to handle simple command seperately then
//	because they at least need to fork in execute_external or my program quits).
//Or modify execute_pipelines to not fork(difficult),
//or create a function like execute external that doesnt fork(new selfwritten execute_external_pipe function)
static void	execute_pipeline(t_ast *node, int *exit_status)
{
	pid_t	pid;
	int		pipe_fds[2];
	int		saved_stdin;
	int		saved_stdout;
	int		status;

	if (node->left)
	{
		if (pipe(pipe_fds) == -1)
			ft_error("minishell", "pipe", strerror(errno));
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		handle_redirection(node);
		pid = fork();
		if (pid == 0)
		{
			close(pipe_fds[0]);
			dup2(pipe_fds[1], STDOUT_FILENO);
			close(pipe_fds[1]);
			execute_ast(node->left, exit_status);
			exit(*exit_status);
		}
		else if (pid != 0)
		{
			close(pipe_fds[1]);
			dup2(pipe_fds[0], STDIN_FILENO);
			close(pipe_fds[0]);
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				*exit_status = WEXITSTATUS(status);
			if (node->right)
				execute_pipeline(node->right, exit_status);
		}
	}
	else
		execute_simple_command(node, exit_status);
}

int	execute_ast(t_ast *ast, int *exit_status)
{
	if (ast->type == AST_SIMPLE_COMMAND)
	{
		handle_redirection(ast);
		execute_simple_command(ast, exit_status);
		return (0);
	}
	else if (ast->type == AST_PIPELINE)
	{
		execute_pipeline(ast, exit_status);
		return (0);
	}
	return (1);
}
