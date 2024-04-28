/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:34:31 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/28 21:46:32 by cpuiu            ###   ########.fr       */
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

t_ast	*set_new_nodes(t_ast *node, t_ast *new_node)
{
	new_node = create_ast_node(node->type, NULL);
	new_node->redirection_mode = node->redirection_mode;
	new_node->redirection_file = ft_strdup(node->redirection_file);
	return (new_node);
}

static void	process_redirection_nodes(t_ast **node_ptr, t_ast ***heredoc_tail,
		t_ast ***file_tail)
{
	t_ast	*node;
	int		mode;
	t_ast	*new_node;

	node = *node_ptr;
	while (node && node->type == AST_REDIRECTION)
	{
		mode = node->redirection_mode;
		new_node = set_new_nodes(node, new_node);
		if (mode == REDIR_HEREDOC)
		{
			**heredoc_tail = new_node;
			*heredoc_tail = &(new_node->right);
		}
		else
		{
			**file_tail = new_node;
			*file_tail = &(new_node->right);
		}
		if (node->right && node->right->type == AST_WHITESPACE)
			node = node->right->right;
		else
			node = node->right;
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
	while (node && node->type != AST_REDIRECTION)
		node = node->right;
	if (!node)
		return (0);
	process_redirection_nodes(&node, &heredoc_tail, &file_tail);
	result = execute_heredoc_list(heredoc_list_ptr);
	free_ast(heredoc_list_ptr);
	if (result != 0)
		return (result);
	result = execute_file_redirection_list(&file_list_ptr);
	free_ast(file_list_ptr);
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
