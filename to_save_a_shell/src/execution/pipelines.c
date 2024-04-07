/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipelines.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 05:09:17 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/07 11:46:31 by mleibeng         ###   ########.fr       */
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

void	execute_command_in_child(int i, int num_pipes, int *pipe_fds,
		t_ast *curr, int *exit_status)
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

void	fork_and_execute_commands_in_pipeline(t_ast *node, int num_pipes,
		int *pipe_fds, int *exit_status)
{
	int		i;
	pid_t	pid;
	t_ast	*curr;

	i = 0;
	curr = node;
	while (i <= num_pipes)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
			execute_command_in_child(i, num_pipes, pipe_fds, curr, exit_status);
		curr = curr->right;
		i++;
	}
}

void	initialize_pipeline_pipes(int num_pipes, int *pipe_fds)
{
	int	i;

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
}

int	*prepare_pipeline_fds(t_ast *node, int *num_pipes)
{
	t_ast	*curr;
	int		*pipe_fds;

	*num_pipes = 0;
	curr = node;
	while (curr->right && curr->type == AST_PIPELINE)
	{
		(*num_pipes)++;
		curr = curr->right;
	}
	pipe_fds = malloc(2 * (*num_pipes) * sizeof(int));
	if (pipe_fds == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (pipe_fds);
}

void	execute_pipeline(t_ast *node, int *exit_status)
{
	int	num_pipes;
	int	*pipe_fds;
	int	i;

	if (node->type == AST_PIPELINE)
	{
		pipe_fds = prepare_pipeline_fds(node, &num_pipes);
		initialize_pipeline_pipes(num_pipes, pipe_fds);
		fork_and_execute_commands_in_pipeline(node, num_pipes, pipe_fds,
			exit_status);
		close_pipes(pipe_fds, num_pipes);
		i = 0;
		while (i <= num_pipes)
		{
			wait_and_update_status(-1, exit_status);
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
