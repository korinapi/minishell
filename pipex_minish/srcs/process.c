/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 15:51:23 by cpuiu             #+#    #+#             */
/*   Updated: 2024/02/06 19:38:04 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	child_1(t_pipex pipex, char **argv, char **envp)
{
	pipex.in_fd = open(argv[1], O_RDONLY);
	if (pipex.in_fd < 0)
	{
		if (errno == EACCES)
			error_message(ERROR_FILE_PERMISSIONS);
		else
			error_message(ERROR_INPUT);
	}
	dup2(pipex.fd[1], 1);
	close(pipex.fd[0]);
	dup2(pipex.in_fd, 0);
	close(pipex.fd[1]);
	pipex.cmd_args = ft_split_quotes(argv[2]);
	if (access(pipex.cmd_args[0], X_OK) == 0)
		pipex.cmd_path = pipex.cmd_args[0];
	else
		pipex.cmd_path = get_cmd_path(pipex.cmd_paths, pipex.cmd_args[0]);
	if (!pipex.cmd_path)
	{
		error_message_cmd(ERROR_CMD, pipex.cmd_args);
		free_process(&pipex, "child");
	}
	execve(pipex.cmd_path, pipex.cmd_args, envp);
	if (execve(pipex.cmd_path, pipex.cmd_args, envp) == -1)
		exit(EXIT_FAILURE);
}

void	child_2(t_pipex pipex, char **argv, char **envp)
{
	pipex.out_fd = open(argv[4], O_TRUNC | O_CREAT | O_RDWR, 0644);
	if (pipex.out_fd < 0)
	{
		if (errno == EACCES)
			error_message(ERROR_FILE_PERMISSIONS);
		else
			error_message_output(argv[4]);
	}
	dup2(pipex.fd[0], 0);
	close(pipex.fd[1]);
	dup2(pipex.out_fd, 1);
	pipex.cmd_args = ft_split_quotes(argv[3]);
	if (access(pipex.cmd_args[0], X_OK) == 0)
		pipex.cmd_path = pipex.cmd_args[0];
	else
		pipex.cmd_path = get_cmd_path(pipex.cmd_paths, pipex.cmd_args[0]);
	if (!pipex.cmd_path)
	{
		error_message_cmd(ERROR_CMD, pipex.cmd_args);
		free_process(&pipex, "child");
	}
	execve(pipex.cmd_path, pipex.cmd_args, envp);
	if (execve(pipex.cmd_path, pipex.cmd_args, envp) == -1)
		exit(EXIT_FAILURE);
}

char	*get_cmd_path(char **paths, char *cmd)
{
	char	*buff;
	char	*cmmd;

	while (*paths)
	{
		buff = ft_strjoin(*paths, "/");
		cmmd = ft_strjoin(buff, cmd);
		free(buff);
		if (access(cmmd, 0) == 0)
			return (cmmd);
		free(cmmd);
		paths++;
	}
	return (NULL);
}
