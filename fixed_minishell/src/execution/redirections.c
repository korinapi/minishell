/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:34:31 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/19 09:37:42 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"
#include "executor.h"
#include "minishell.h"

static char	*generate_tmp_file_name(void)
{
	static const char	tmp_dir[] = "/tmp/minishell_";
	char				*tmp_file;
	unsigned char		random_bytes[12];
	int					read_result;
	int					fd;
	unsigned int		i;

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

int	create_temp_file(char **tmp_file)
{
	int	fd;

	*tmp_file = generate_tmp_file_name();
	fd = open(*tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_error("minishell", *tmp_file, strerror(errno));
		free(*tmp_file);
		*tmp_file = NULL;
		return (-1);
	}
	return (fd);
}
void	read_heredoc_input(int fd, const char *end_marker)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = readline("heredoc> ");
		if (line == NULL || !ft_strcmp(line, end_marker))
		{
			printf(line == NULL ? "EOF or error encountered. Exiting heredoc.\n" : "");
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}
int	redirect_stdin_from_file(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("Error reopening heredoc temp file");
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	execute_heredoc(t_ast *node)
{
	char	*tmp_file;
	int		fd;

	fd = create_temp_file(&tmp_file);
	if (fd == -1)
		return (1);
	read_heredoc_input(fd, node->redirection_file);
	close(fd);
	if (redirect_stdin_from_file(tmp_file) != 0)
	{
		free(tmp_file);
		return (1);
	}
	free(tmp_file);
	return (0);
}

int	execute_redirections_fds(t_ast *node)
{
	t_redirection	*redir;
	int				fd;
	int				flags;

	for (int i = 0; i < node->num_redirections; i++)
	{
		redir = &node->redirections[i];
		if (redir->mode == REDIR_OUT)
			flags = O_CREAT | O_WRONLY | O_TRUNC;
		else if (redir->mode == REDIR_OUT_APPEND)
			flags = O_CREAT | O_WRONLY | O_APPEND;
		else if (redir->mode == REDIR_IN)
			flags = O_RDONLY;
		else
			continue ;
		fd = open(redir->filename, flags, 0644);
		if (fd == -1)
		{
			perror("Error opening file");
			return (errno);
		}
		if (redir->mode == REDIR_OUT || redir->mode == REDIR_OUT_APPEND)
			dup2(fd, STDOUT_FILENO);
		else if (redir->mode == REDIR_IN)
			dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (0);
}

int	execute_redirection(t_ast *node)
{
	int	mode;

	int	fd;
	int	flags;
	mode = node->redirection_mode;
	if (mode == REDIR_HEREDOC)
	{
		if (execute_heredoc(node) != 0)
			return (1);
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
			perror("Permission denied");
			exit(1);
		}
		if (mode == REDIR_OUT || mode == REDIR_OUT_APPEND)
			dup2(fd, STDOUT_FILENO);
		else
			dup2(fd, STDIN_FILENO);
		close(fd);
		//execute_redirections_fds(node);
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
