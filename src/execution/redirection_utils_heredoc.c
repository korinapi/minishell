/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils_heredoc.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 21:00:57 by cpuiu             #+#    #+#             */
/*   Updated: 2024/04/24 09:13:17 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"
#include "executor.h"
#include "minishell.h"
#include "redirection.h"

void	read_heredoc_input(int fd, const char *end_marker)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (line == NULL || !ft_strcmp(line, end_marker))
		{
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

int	execute_heredoc_list(t_ast *heredoc_node)
{
	int		result;
	char	*tmp_file;
	int		fd;

	result = 0;
	tmp_file = NULL;
	fd = -1;
	while (heredoc_node && heredoc_node->redirection_file != NULL)
	{
		fd = create_temp_file(&tmp_file);
		if (fd == -1)
			return (1);
		read_heredoc_input(fd, heredoc_node->redirection_file);
		heredoc_node = heredoc_node->right;
	}
	if (fd != -1)
	{
		close(fd);
		result = redirect_stdin_from_file(tmp_file);
		free(tmp_file);
	}
	return (result);
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
