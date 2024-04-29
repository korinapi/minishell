/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_redir_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 18:04:58 by cpuiu             #+#    #+#             */
/*   Updated: 2024/04/29 18:28:46 by mleibeng         ###   ########.fr       */
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

void	wait_loop(int num_pipes, int *exit_status, int cmd_ex_stat)
{
	int	i;

	i = 0;
	while (i <= num_pipes)
	{
		*exit_status = wait_and_update_status(-1);
		if (*exit_status == 0 && cmd_ex_stat != 0)
			*exit_status = cmd_ex_stat;
		i++;
	}
}
