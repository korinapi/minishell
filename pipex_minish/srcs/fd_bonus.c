/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_bonus.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 12:06:08 by cpuiu             #+#    #+#             */
/*   Updated: 2024/02/06 21:13:00 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	*find_path(char **envp)
{
	if (envp == NULL || envp[0] == NULL)
		return (ft_strdup("/bin:/usr/bin"));
	while (ft_strncmp("PATH", *envp, 4))
		envp++;
	return (*envp + 5);
}

void	get_infile(char **argv, t_pipex_bonus *pipex)
{
	if (!ft_strncmp("here_doc", argv[1], 9))
		here_doc(argv[2], pipex);
	else
	{
		pipex->in_fd = open(argv[1], O_RDONLY);
		if (pipex->in_fd < 0)
		{
			if (errno == EACCES)
				error_message(ERROR_FILE_PERMISSIONS);
			else
				error_message(ERROR_INPUT);
		}
	}
}

void	get_outfile(char *argv, t_pipex_bonus *pipex)
{
	if (pipex->here_doc)
		pipex->out_fd = open(argv, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		pipex->out_fd = open(argv, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (pipex->out_fd < 0)
	{
		if (errno == EACCES)
			error_message(ERROR_FILE_PERMISSIONS);
		else
			error_message(&argv[pipex->arg_nb - 1]);
	}
}
