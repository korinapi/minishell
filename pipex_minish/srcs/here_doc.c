/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 12:17:21 by cpuiu             #+#    #+#             */
/*   Updated: 2024/02/06 20:29:45 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	here_doc(char *argv, t_pipex_bonus *pipex)
{
	int		file;
	char	*buff;

	file = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0000644);
	if (file < 0)
		errors(ERROR_HERE_DOC);
	while (true)
	{
		ft_putstr_fd("heredoc> ", 1);
		if (get_next_line(0, &buff) < 0)
			exit(EXIT_FAILURE);
		if (!ft_strncmp(argv, buff, ft_strlen(argv) + 1))
			break ;
		ft_putendl_fd(buff, file);
		free(buff);
	}
	free(buff);
	close(file);
	pipex->in_fd = open(".heredoc_tmp", O_RDONLY);
	if (pipex->in_fd < 0)
	{
		unlink(".heredoc_tmp");
		errors(ERROR_HERE_DOC);
	}
	pipex->here_doc = 1;
}
