/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:27:11 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/07 06:40:18 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "parser.h"
# include "minishell.h"

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

void			execute_ast(t_ast *ast, int *exit_status);
int				execute_builtin(t_ast *ast, int *exit_status);
void		execute_pipeline(t_ast *node, int *exit_status);
void		execute_simple_command(t_ast *node, int *exit_status);
int		execute_external(t_ast *ast, int *exit_status);
void	close_pipes(int *pipe_fds, int num_pipes);
void	execute_pipeline(t_ast *node, int *exit_status);
int		*prepare_pipeline_fds(t_ast *node, int *num_pipes);
void	initialize_pipeline_pipes(int num_pipes, int *pipe_fds);
void	fork_and_execute_commands_in_pipeline(t_ast *node, int num_pipes,
			int *pipe_fds, int *exit_status);
void	execute_command_in_child(int i, int num_pipes, int *pipe_fds,
			t_ast *curr, int *exit_status);
char	*find_command_path(char *command, char **paths);
void	execute_command_from_path(char **args);
void	wait_and_update_status(pid_t pid, int *exit_status);
void	execute_simple_command(t_ast *node, int *exit_status);
#endif