/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipelines.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 05:09:17 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/07 05:21:17 by mleibeng         ###   ########.fr       */
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

void	execute_pipeline(t_ast *node, int *exit_status)
{
	t_pipehelper	pipehelper;
	int				num_pipes;
	int				i;

	if (node->type == AST_PIPELINE)
	{
		num_pipes = 0;
		pipehelper.curr = node;
		while (pipehelper.curr->right && pipehelper.curr->type == AST_PIPELINE)
		{
			num_pipes++;
			pipehelper.curr = pipehelper.curr->right;
		}
		pipehelper.pipe_fds = malloc(2 * num_pipes * sizeof(int));
		if (pipehelper.pipe_fds == NULL)
		{
			ft_fprintf(STDERR_FILENO, "malloc failed: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		i = 0;
		while (i < num_pipes)
		{
			if (pipe(pipehelper.pipe_fds + 2 * i) == -1)
			{
				ft_fprintf(STDERR_FILENO, "pipe failed: %s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			i++;
		}
		pipehelper.curr = node;
		pipehelper.pid = 0;
		i = 0;
		while (i <= num_pipes)
		{
			pipehelper.pid = fork();
			if (pipehelper.pid == -1)
			{
				perror("fork");
				exit(EXIT_FAILURE);
			}
			else if (pipehelper.pid == 0)
			{
				if (i > 0)
				{
					dup2(pipehelper.pipe_fds[2 * (i - 1)], STDIN_FILENO);
					close(pipehelper.pipe_fds[2 * (i - 1)]);
				}
				if (i < num_pipes)
				{
					dup2(pipehelper.pipe_fds[2 * i + 1], STDOUT_FILENO);
					close(pipehelper.pipe_fds[2 * i + 1]);
				}
				close_pipes(pipehelper.pipe_fds, num_pipes);
				handle_redirection(pipehelper.curr->left);
				execute_simple_command(pipehelper.curr->left, exit_status);
				exit(EXIT_SUCCESS);
			}
			pipehelper.curr = pipehelper.curr->right;
			i++;
		}
		close_pipes(pipehelper.pipe_fds, num_pipes);
		i = 0;
		while (i <= num_pipes)
		{
			waitpid(pipehelper.pid, &pipehelper.status, 0);
			if (WIFEXITED(pipehelper.status))
				*exit_status = WEXITSTATUS(pipehelper.status);
			i++;
		}
		free(pipehelper.pipe_fds);
	}
	else
	{
		handle_redirection(node);
		execute_simple_command(node, exit_status);
	}
}
