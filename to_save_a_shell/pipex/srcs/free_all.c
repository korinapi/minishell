/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 16:07:59 by cpuiu             #+#    #+#             */
/*   Updated: 2024/02/28 08:46:17 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_process(t_pipex *pipex, char *process)
{
	int	i;

	i = 0;
	if (ft_strncmp(process, "child", 5) == 0)
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
	else if (ft_strncmp(process, "parent", 6) == 0)
	{
		while (pipex->cmd_paths[i])
		{
			free(pipex->cmd_paths[i]);
			i++;
		}
		free(pipex->cmd_paths);
	}
}
