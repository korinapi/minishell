/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipelines.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 05:09:17 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/29 18:26:12 by mleibeng         ###   ########.fr       */
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

void	handle_redirections(int i, int num_pipes, t_pipehelper p_help)
{
	if (i > 0)
	{
		dup2(p_help.pipe_fds[2 * (i - 1)], STDIN_FILENO);
		close(p_help.pipe_fds[2 * (i - 1)]);
	}
	if (i < num_pipes)
	{
		dup2(p_help.pipe_fds[2 * i + 1], STDOUT_FILENO);
		close(p_help.pipe_fds[2 * i + 1]);
	}
}

void	fork_and_execute_commands_in_pipeline(int num_pipes,
		t_pipehelper p_helper, int *exit_status, char ***envp)
{
	int		i;
	pid_t	pid;

	i = 0;
	setup_child_signals();
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
			handle_redirections(i, num_pipes, p_helper);
			close_pipes(p_helper.pipe_fds, num_pipes);
			execute_simple_command_without_forks(p_helper.curr->left,
				exit_status, envp);
			exit(*exit_status);
		}
		p_helper.curr = p_helper.curr->right;
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

void	execute_pipeline(t_ast *node, int *exit_status, char ***envp)
{
	int				num_pipes;
	int				*pipe_fds;
	t_pipehelper	p_helper;
	int				cmd_ex_stat;

	if (node->type == AST_PIPELINE)
	{
		pipe_fds = prepare_pipeline_fds(node, &num_pipes);
		initialize_pipeline_pipes(num_pipes, pipe_fds);
		p_helper.curr = node;
		p_helper.pipe_fds = pipe_fds;
		cmd_ex_stat = 0;
		fork_and_execute_commands_in_pipeline(num_pipes, p_helper, exit_status,
			envp);
		close_pipes(pipe_fds, num_pipes);
		wait_loop(num_pipes, exit_status, cmd_ex_stat);
		setup_signals();
		free(pipe_fds);
	}
	else
		execute_simple_command(node, exit_status, envp);
}
