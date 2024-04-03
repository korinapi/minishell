/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 11:50:06 by cpuiu             #+#    #+#             */
/*   Updated: 2024/03/13 18:45:43 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	duplicating(t_pipex_bonus pipex)
{
	if (pipex.index == 0)
	{
		dup2(pipex.in_fd, 0);
		dup2(pipex.pip[1], 1);
	}
	else if (pipex.index == pipex.arg_nb - 1)
	{
		dup2(pipex.pip[2 * pipex.index - 2], 0);
		dup2(pipex.out_fd, 1);
	}
	else
	{
		dup2(pipex.pip[2 * pipex.index - 2], 0);
		dup2(pipex.pip[2 * pipex.index + 1], 1);
	}
}

void	child(t_pipex_bonus pipex, char **argv, char **envp)
{
	pipex.pid = fork();
	if (!pipex.pid)
	{
		duplicating(pipex);
		close_pipes(&pipex);
		pipex.cmd_args = ft_split_quotes(argv[2 + pipex.here_doc
				+ pipex.index]);
		if (access(pipex.cmd_args[0], X_OK) == 0)
			pipex.cmd = pipex.cmd_args[0];
		else
			pipex.cmd = get_cmd_path(pipex.cmd_paths, pipex.cmd_args[0]);
		if (!pipex.cmd)
		{
			errors(pipex.cmd_args[0]);
			child_free(&pipex);
			exit(EXIT_FAILURE);
		}
		if (execve(pipex.cmd, pipex.cmd_args, envp) == -1)
			exit(EXIT_FAILURE);
	}
}
