/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 16:07:59 by cpuiu             #+#    #+#             */
/*   Updated: 2024/02/06 20:29:31 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	free_process(t_pipex *pipex, char *process)
{
	int	i;

	i = 0;
	if (ft_strcmp(process, "child") == 0)
	{
		close(pipex->in_fd);
		while (pipex->cmd_args[i])
		{
			free(pipex->cmd_args[i]);
			i++;
		}
		free(pipex->cmd_args);
		free(pipex->cmd_path);
	}
	else if (ft_strcmp(process, "parent") == 0)
	{
		while (pipex->cmd_paths[i])
		{
			free(pipex->cmd_paths[i]);
			i++;
		}
		free(pipex->cmd_paths);
	}
}
