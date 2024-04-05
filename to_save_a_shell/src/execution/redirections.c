/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:34:31 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/05 20:55:47 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"
#include "executor.h"
#include "minishell.h"

static char	*generate_tmp_file_name(void)
{
	char	*template;
	int		fd;

	// static const char	tmp_dir[] = "/tmp/minishell_";
	// char				*tmp_file;
	// int					read_result;
	// int					fd;
	// tmp_file = malloc(sizeof(tmp_dir) + 12);
	// if (!tmp_file)
	// 	return (NULL);
	// ft_strcpy(tmp_file, tmp_dir);
	// fd = open("/dev/urandom", O_RDONLY);
	// if (fd == -1)
	// {
	// 	free(tmp_file);
	// 	return (NULL);
	// }
	// read_result = read(fd, tmp_file + ft_strlen(tmp_dir), 12);
	// (void)read_result;
	// close(fd);
	// tmp_file[ft_strlen(tmp_file)] = '\0';
	// return (tmp_file);
	template = strdup("/tmp/minishell_heredoc_XXXXXX");
	if (!template)
	{
		perror("strdup failed");
		exit(EXIT_FAILURE);
	}
	fd = mkstemp(template);
	if (fd == -1)
	{
		perror("mkstemp failed");
		free(template);
		exit(EXIT_FAILURE);
	}
	close(fd);
	return (template);
}

void	handle_heredoc_redirection(t_ast *node)
{
	char	*tmp_file;
	int		fd;
	char	*line;

	tmp_file = generate_tmp_file_name();
	printf("Generated temp file name: %s\n", tmp_file);
	fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_error("Error opening temp file", tmp_file, strerror(errno));
		free(tmp_file);
		return ;
	}
	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (!ft_strcmp(line, node->heredoc_delimiter))
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	fd = open(tmp_file, O_RDONLY);
	if (fd == -1)
	{
		ft_error("Error opening temp file", tmp_file, strerror(errno));
		free(tmp_file);
		return ;
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	free(tmp_file);
}

void	execute_redirection(t_ast *node)
{
	int	mode;
	int	fd;
	int	flags;

	// char	*line;
	// char	*tmp_file;
	mode = node->redirection_mode;
	if (mode == REDIR_HEREDOC)
	{
		handle_heredoc_redirection(node);
		// tmp_file = generate_tmp_file_name();
		// fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		// line = NULL;
		// while (1)
		// {
		// 	line = readline("> ");
		// 	if (!ft_strcmp(line, node->heredoc_delimiter))
		// 		break ;
		// 	ft_putendl_fd(line, fd);
		// 	free(line);
		// }
		// free(line);
		// close(fd);
		// fd = open(tmp_file, O_RDONLY);
		// dup2(fd, STDIN_FILENO);
		// close(fd);
		// free(tmp_file);
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
			ft_error("minishell", node->redirection_file, strerror(errno));
			return ;
		}
		if (mode == REDIR_OUT || mode == REDIR_OUT_APPEND)
			dup2(fd, STDOUT_FILENO);
		else
			dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

int	handle_redirection(t_ast *node)
{
	if (node == NULL)
		return (0);
	if (node->type == AST_REDIRECTION)
		execute_redirection(node);
	else
	{
		handle_redirection(node->left);
		handle_redirection(node->right);
	}
	return (0);
}
