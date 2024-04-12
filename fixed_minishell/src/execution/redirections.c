/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:34:31 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/12 13:46:42 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"
#include "executor.h"
#include "minishell.h"

static char *generate_tmp_file_name(void)
{
	static const char tmp_dir[] = "/tmp/minishell_";
	char *tmp_file;
	unsigned char random_bytes[12];
	int read_result;
	int fd;
	unsigned int i;

	tmp_file = malloc(sizeof(tmp_dir) + 24 + 1);
	if (!tmp_file)
		return (NULL);
	ft_strcpy(tmp_file, tmp_dir);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
	{
		free(tmp_file);
		return (NULL);
	}
	read_result = read(fd, random_bytes, sizeof(random_bytes));
	close(fd);
	if (read_result != sizeof(random_bytes))
	{
		free(tmp_file);
		return (NULL);
	}
	for (i = 0; i < sizeof(random_bytes); i++)
		sprintf(tmp_file + strlen(tmp_file), "%02x", random_bytes[i]);
	return (tmp_file);
}

int	execute_redirection(t_ast *node)
{
	int mode;
	int fd;
	int flags;
	char *line;
	char *tmp_file;

	mode = node->redirection_mode;
	if (mode == REDIR_HEREDOC)
	{
		tmp_file = generate_tmp_file_name();
		fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_error("minishell", tmp_file, strerror(errno));
			free(tmp_file);
			return (1);
		}
		printf("Starting heredoc input. Type '%s' to end.\n", node->redirection_file);
		line = NULL;
		while (1)
		{
			line = readline("heredoc> ");
			if (line == NULL)
			{
				printf("EOF or error encountered. Exiting heredoc.\n");
				break ;
			}
			if (!ft_strcmp(line, node->redirection_file))
				break ;
			ft_putendl_fd(line, fd);
			free(line);
		}
		free(line);
		close(fd);
		fd = open(tmp_file, O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
		free(tmp_file);
	}
	else
	{
		if (mode == REDIR_OUT)
			flags = O_CREAT | O_WRONLY | O_TRUNC;
		else if (mode == REDIR_OUT_APPEND)
			flags = O_CREAT | O_WRONLY | O_APPEND;
		else
			flags = O_RDONLY;
		fd = open(node->redirection_file, flags, 0644);
		if (fd == -1)
		{
			// ft_error("minishell", node->redirection_file, strerror(errno));
			return (errno);
		}
		if (mode == REDIR_OUT || mode == REDIR_OUT_APPEND)
			dup2(fd, STDOUT_FILENO);
		else
			dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (0);
}

int	handle_redirection(t_ast *node, int *exit_status)
{
	int	val;

	if (node == NULL)
		return (0);
	if (node->type == AST_REDIRECTION)
	{
		val = execute_redirection(node);
		if (val != 0)
		{
			*exit_status = val;
			return (1);
		}
	}
	else
	{
		if (handle_redirection(node->left, exit_status))
			return (1);
		if (handle_redirection(node->right, exit_status))
			return (1);
	}
	return (0);
}

bool	has_redirection_nodes(t_ast *node)
{
	if (node == NULL)
	{
		return (false);
	}

	if (node->type == AST_REDIRECTION)
	{
		return (true);
	}

	return (has_redirection_nodes(node->left)
		|| has_redirection_nodes(node->right));
}
