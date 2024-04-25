/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvinleibenguth <marvinleibenguth@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:34:31 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/25 12:51:45 by marvinleibe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"
#include "executor.h"
#include "minishell.h"
#include "redirection.h"

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
	{
		ft_error("minishell", node->redirection_file,
			"No such file or directory");
		return (1);
	}
	if (mode == REDIR_OUT || mode == REDIR_OUT_APPEND)
		dup2(fd, STDOUT_FILENO);
	else if (mode == REDIR_IN)
		dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static t_ast *skip_whitespace_nodes(t_ast *node)
{
    t_ast *temp;
    while (node && node->type == AST_WHITESPACE)
    {
        temp = node;
        node = node->right;
        free(temp);
    }
    return (node);
}

static void	process_redirection_nodes(t_ast **node_ptr, t_ast ***heredoc_tail,
		t_ast ***file_tail)
{
	t_ast	*node;
	int		mode;

	node = *node_ptr;
	while (node && node->type == AST_REDIRECTION)
	{
		mode = node->redirection_mode;
		if (mode == REDIR_HEREDOC)
		{
			**heredoc_tail = node;
			*heredoc_tail = &(node->right);
		}
		else
		{
			**file_tail = node;
			*file_tail = &(node->right);
		}
		node = skip_whitespace_nodes(node->right);
	}
	*node_ptr = node;
}

int	execute_redirection(t_ast *node)
{
	int		result;
	t_ast	*heredoc_list_ptr;
	t_ast	*file_list_ptr;
	t_ast	**heredoc_tail;
	t_ast	**file_tail;

	heredoc_list_ptr = NULL;
	heredoc_tail = &heredoc_list_ptr;
	heredoc_list_ptr = NULL;
	file_list_ptr = NULL;
	heredoc_tail = &heredoc_list_ptr;
	file_tail = &file_list_ptr;
	node = skip_whitespace_nodes(node);
	while (node && node->type != AST_REDIRECTION)
		node = node->right;
	process_redirection_nodes(&node, &heredoc_tail, &file_tail);
	result = execute_heredoc_list(heredoc_list_ptr);
	if (result != 0)
		return (result);
	result = execute_file_redirection_list(&file_list_ptr);
	return (result);
}

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
