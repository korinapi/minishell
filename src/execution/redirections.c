/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:34:31 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/22 11:04:47 by cpuiu            ###   ########.fr       */
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
	int					fd;
	size_t				i;
	int					offset;
	size_t				tmp_dir_len;

	tmp_dir_len = sizeof(tmp_dir) - 1;
	tmp_file = malloc(tmp_dir_len + 24 * 2 + 1);
	if (!tmp_file)
		return (NULL);
	ft_strcpy(tmp_file, tmp_dir);
	offset = tmp_dir_len;
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
	{
		free(tmp_file);
		return (NULL);
	}
	if (read(fd, random_bytes, sizeof(random_bytes)) != sizeof(random_bytes))
	{
		close(fd);
		free(tmp_file);
		return (NULL);
	}
	close(fd);
	for (i = 0; i < sizeof(random_bytes); i++)
	{
		snprintf(tmp_file + offset, 3, "%02x", random_bytes[i]);
		offset += 2;
	}
	tmp_file[offset] = '\0';
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

int	execute_file_redirection(t_ast *node)
{
	int	mode;
	int	fd;
	int	flags;

	mode = node->redirection_mode;
	if (mode == REDIR_OUT)
		flags = O_CREAT | O_WRONLY | O_TRUNC;
	else if (mode == REDIR_OUT_APPEND)
		flags = O_CREAT | O_WRONLY | O_APPEND;
	else
		flags = O_RDONLY;
	fd = open(node->redirection_file, flags, 0644);
	if (fd == -1)
		return (1);
	if (mode == REDIR_OUT || mode == REDIR_OUT_APPEND)
		dup2(fd, STDOUT_FILENO);
	else if (mode == REDIR_IN)
		dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	execute_file_redirection_list(t_ast **file_node_ptr)
{
	int		result;
	t_ast	*current_node;

	result = 0;
	current_node = *file_node_ptr;
	while (current_node && current_node->redirection_file != NULL)
	{
		result = execute_file_redirection(current_node);
		if (result != 0)
			return (result);
		current_node = current_node->right;
	}
	return (0);
}

int	execute_redirection(t_ast *node)
{
	int		result;
	t_ast	*heredoc_list_ptr;
	t_ast	*file_list_ptr;
	t_ast	**heredoc_tail;
	t_ast	**file_tail;
	int		mode;

	result = 0;
	heredoc_list_ptr = NULL;
	file_list_ptr = NULL;
	heredoc_tail = &heredoc_list_ptr;
	file_tail = &file_list_ptr;
	while (node && node->type != AST_REDIRECTION)
		node = node->right;
	while (node && node->type == AST_REDIRECTION)
	{
		mode = node->redirection_mode;
		if (mode == REDIR_HEREDOC)
		{
			*heredoc_tail = node;
			heredoc_tail = &(node->right);
		}
		else
		{
			*file_tail = node;
			file_tail = &(node->right);
		}
		if (node->right && node->right->type == AST_WHITESPACE)
			node = node->right->right;
		else
			node = node->right;
	}
	result = execute_heredoc_list(heredoc_list_ptr);
	if (result != 0)
		return (result);
	result = execute_file_redirection_list(&file_list_ptr);
	if (result != 0)
		return (result);
	return (0);
}

// int execute_file_redirection_list(file_node)
// {

// }

// int	execute_redirection(t_ast *node)
// {
// 	int	mode;
// 	int	fd;
// 	int	flags;
// 	t_ast *heredoc_list;
// 	t_ast *file_list;

// 	heredoc_list = NULL;
// 	file_list = NULL;
// 	while (node && (node->type == AST_WHITESPACE
// || node->type == AST_REDIRECTION))
// 	{
//         mode = node->redirection_mode;
//         if (node->type == AST_REDIRECTION)
// 		{
//             if (mode == REDIR_HEREDOC)
// 			{
//                 node->right = heredoc_list;
//                 heredoc_list = node;
//             }
// 			else
// 			{
//                 node->right = file_list;
//                 file_list = node;
//             }
//         }
//         node = node->right;
//     }

// 	while ((node && node->type == AST_WHITESPACE) || (node
// 			&& node->type == AST_REDIRECTION))
// 	{
// 		mode = node->redirection_mode;
// 		if (node->type == AST_WHITESPACE)
// 		{
// 			node = node->right;
// 			continue ;
// 		}
// 		if (node->type == AST_REDIRECTION)
// 		{
// 			if (mode == REDIR_HEREDOC)
// 			{
// 				if (execute_heredoc(node))
// 					return (1);
// 				return (0);
// 			}
// 			if (mode == REDIR_OUT)
// 				flags = O_CREAT | O_WRONLY | O_TRUNC;
// 			else if (mode == REDIR_OUT_APPEND)
// 				flags = O_CREAT | O_WRONLY | O_APPEND;
// 			else
// 				flags = O_RDONLY;
// 			fd = open(node->redirection_file, flags, 0644);
// 			if (fd == -1)
// 				return (1);
// 			if (node->right == NULL || node->right->type != AST_REDIRECTION
// 				|| (node->right->type == AST_REDIRECTION
// 					&& node->right->redirection_mode != REDIR_IN))
// 			{
// 				if (mode == REDIR_OUT || mode == REDIR_OUT_APPEND)
// 					dup2(fd, STDOUT_FILENO);
// 				else if (mode == REDIR_IN)
// 					dup2(fd, STDIN_FILENO);
// 				close(fd);
// 			}
// 		}
// 		node = node->right;
// 	}
// 	return (0);
// }

int	handle_redirection(t_ast *node, int *exit_status)
{
	int	val;

	while (node)
	{
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
			break ;
		}
		node = node->right;
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
