/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:29:28 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/05 02:48:27 by mleibeng         ###   ########.fr       */
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

void	close_pipes(int *pipe_fds, int num_pipes)
{
	int i;

	i = 0;
	while (i < 2 * num_pipes)
	{
		close(pipe_fds[i]);
		i++;
	}
}

void	execute_pipeline(t_ast *node, int *exit_status)
{
	int		num_pipes;
	t_ast	*curr;
	int		*pipe_fds;
	int		pid;
	int		status;
	int		i;

	if (node->type == AST_PIPELINE)
	{
		num_pipes = 0;
		curr = node;
		while (curr->right && curr->type == AST_PIPELINE)
		{
			num_pipes++;
			curr = curr->right;
		}
		pipe_fds = malloc(2 * num_pipes * sizeof(int));
		if (pipe_fds == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		i = 0;
		while (i < num_pipes)
		{
			if (pipe(pipe_fds + 2 * i) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
			i++;
		}
		curr = node;
		pid = 0;
		i = 0;
		while (i <= num_pipes)
		{
			pid = fork();
			if (pid == -1)
			{
				perror("fork");
				exit(EXIT_FAILURE);
			}
			else if (pid == 0)
			{
				if (i > 0)
				{
					dup2(pipe_fds[2 * (i - 1)], STDIN_FILENO);
					close(pipe_fds[2 * (i - 1)]);
				}
				if (i < num_pipes)
				{
					dup2(pipe_fds[2 * i + 1], STDOUT_FILENO);
					close(pipe_fds[2 * i + 1]);
				}
				close_pipes(pipe_fds, num_pipes);
				handle_redirection(curr->left);
				execute_simple_command(curr->left, exit_status);
				exit(EXIT_SUCCESS);
			}
			curr = curr->right;
			i++;
		}
		close_pipes(pipe_fds, num_pipes);
		i = 0;
		while (i <= num_pipes)
		{
			waitpid(pid, &status, 0);
			if(WIFEXITED(status))
				*exit_status = WEXITSTATUS(status);
			i++;
		}
		free(pipe_fds);
	}
	else
	{
		handle_redirection(node);
		execute_simple_command(node, exit_status);
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
