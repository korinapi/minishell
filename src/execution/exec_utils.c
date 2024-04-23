/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 19:42:46 by cpuiu             #+#    #+#             */
/*   Updated: 2024/04/23 20:03:42 by mleibeng         ###   ########.fr       */
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

void	handle_specific_error(int *exit_status)
{
	if (*exit_status == ENOENT || *exit_status == ENOTDIR
		|| *exit_status == EACCES)
	{
		ft_fprintf(STDERR_FILENO, " %s\n", strerror(*exit_status));
		exit(1);
	}
}

char	*get_command_path(char *command)
{
	char	*path_env;
	char	**paths;
	char	*command_path;

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	command_path = find_command_path(command, paths);
	ft_free_split(paths);
	return (command_path);
}

void	execute_command_from_path(char **args, char *command_path,
		int *exit_status)
{
	handle_specific_error(exit_status);
	if (execve(command_path, args, environ) == -1)
	{
		ft_fprintf(STDERR_FILENO, "execve failed: %s\n", strerror(errno));
		free(command_path);
		exit(126);
	}
	free(command_path);
}

int	wait_and_update_status(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (WTERMSIG(status));
	else
		return (1);
}
