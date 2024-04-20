/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:27:11 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/10 02:21:29 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
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
int			wait_and_update_status(pid_t pid);
void		execute_command_from_path(char **args, int *exit_status);
char		*find_command_path(char *command, char **paths);
void		close_pipes(int *pipe_fds, int num_pipes);

#endif