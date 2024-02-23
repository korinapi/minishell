/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 11:35:30 by cpuiu             #+#    #+#             */
/*   Updated: 2024/02/06 21:12:41 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	parent_free(t_pipex_bonus *pipex)
{
	int	i;

	i = 0;
	close(pipex->in_fd);
	close(pipex->out_fd);
	if (pipex->here_doc)
		unlink(".heredoc_tmp");
	while (pipex->cmd_paths[i])
	{
		free(pipex->cmd_paths[i]);
		i++;
	}
	free(pipex->cmd_paths);
	free(pipex->pip);
}

void	child_free(t_pipex_bonus *pipex)
{
	int	i;

	i = 0;
	while (pipex->cmd_args[i])
	{
		free(pipex->cmd_args[i]);
		i++;
	}
	free(pipex->cmd_args);
	free(pipex->cmd);
}

void	pipe_free(t_pipex_bonus *pipex)
{
	close(pipex->in_fd);
	close(pipex->out_fd);
	if (pipex->here_doc)
		unlink(".heredoc_tmp");
	free(pipex->pip);
	error_message(ERROR_ENVP);
	exit(1);
}
