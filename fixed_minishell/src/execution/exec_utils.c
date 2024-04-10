/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 19:42:46 by cpuiu             #+#    #+#             */
/*   Updated: 2024/04/10 02:37:46 by mleibeng         ###   ########.fr       */
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

char	*find_command_path(char *command, char **paths)
{
	char	*full_path;
	int		i;

	if (ft_strchr(command, '/') != NULL)
	{
		if (!access(command, X_OK))
			return (ft_strdup(command));
		else
			return (NULL);
	}
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin_free(full_path, command, 1);
		if (!access(full_path, X_OK))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

void	execute_command_from_path(char **args, int *exit_status)
{
	char	*path_env;
	char	**paths;
	char	*command_path;

	if(*exit_status == ENOENT || *exit_status == ENOTDIR)
	{
		ft_fprintf(STDERR_FILENO, " %s\n", strerror(*exit_status));
		exit (1);
	}
	else if (*exit_status == EACCES)
	{
		ft_fprintf(STDERR_FILENO, " %s\n", strerror(*exit_status));
		exit(1);
	}
	path_env = getenv("PATH");
	paths = ft_split(path_env, ':');
	command_path = find_command_path(args[0], paths);
	if (command_path == NULL)
	{
		ft_putstr_fd("Command not found: ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);
		exit(127);
	}
	if (execve(command_path, args, NULL) == -1)
	{
		ft_fprintf(STDERR_FILENO, "execve failed: %s\n", strerror(errno));
		free(command_path);
		exit(126);
	}
	free(command_path);
	exit(1);
}

int	wait_and_update_status(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	else
		return (1);
}

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