/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:27:11 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/07 05:54:36 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "parser.h"

typedef struct s_exechelper
{
	char	**args;
	char	*path_env;
	char	**paths;
	char	*command_path;
	t_ast	*current_node;
}			t_exechelper;

typedef struct s_pipehelper
{
	t_ast	*curr;
	int		*pipe_fds;
	int		pid;
	int		status;
}			t_pipehelper;

void		execute_ast(t_ast *ast, int *exit_status);
int			execute_builtin(t_ast *ast, int *exit_status);
void		execute_pipeline(t_ast *node, int *exit_status);
void		execute_simple_command(t_ast *node, int *exit_status);

#endif