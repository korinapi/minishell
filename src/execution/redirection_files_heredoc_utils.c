/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_files_heredoc_utils.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 18:04:58 by cpuiu             #+#    #+#             */
/*   Updated: 2024/04/29 18:07:55 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"
#include "executor.h"
#include "minishell.h"
#include "redirection.h"

void	handle_heredoc_mode_redirection(t_ast *new_node, t_ast ***heredoc_tail)
{
	**heredoc_tail = new_node;
	*heredoc_tail = &(new_node->right);
}

void	handle_file_mode_redirection(t_ast *new_node, t_ast ***file_tail)
{
	**file_tail = new_node;
	*file_tail = &(new_node->right);
}
