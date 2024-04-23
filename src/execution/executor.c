/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:29:28 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/23 15:25:33 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "environment.h"
#include "errors.h"
#include "executor.h"
#include "free.h"
#include "minishell.h"
#include "redirection.h"
#include "utilities.h"

int	execute_external(char **args, int *exit_status)
{
	pid_t	pid;

	rl_catch_signals = 1;
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	else if (pid == 0)
	{
		execute_command_from_path(args, exit_status);
		return (1);
	}
	else
		*exit_status = wait_and_update_status(pid);
	rl_catch_signals = 0;
	return (*exit_status);
}

void	execute_simple_command(t_ast *node, int *exit_status)
{
	char	**args;
	int		i;
	t_ast	*current_node;

	if (handle_redirection(node->left, exit_status))
		return ;
	args = ft_calloc(ast_count_nodes(node->left) + 1, sizeof(char *));
	i = 0;
	current_node = node->left;
	while (current_node)
	{
		if (current_node->type != AST_WHITESPACE
			&& current_node->type != AST_REDIRECTION)
			args[i++] = current_node->data;
		current_node = current_node->right;
	}
	args[i] = NULL;
	if (is_builtin(args[0]))
		*exit_status = execute_builtin(node, exit_status);
	else
		*exit_status = execute_external(args, exit_status);
	free(args);
}

int	syntax_check(t_ast *ast, int *exit_status)
{
	if (!ast)
		return (1);
	if (ast->type == AST_PIPELINE && !ast->left->left)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n");
		*exit_status = 258;
		return (1);
	}
	// if (ast->type == AST_SIMPLE_COMMAND)
	// {
	// 	if ((ast->left->redirection_mode == REDIR_IN
	// 			|| ast->left->redirection_mode == REDIR_OUT
	// 			|| ast->left->redirection_mode == REDIR_OUT_APPEND
	// 			|| ast->left->redirection_mode == REDIR_HEREDOC))
	// 	{
	// 		if (ft_strcmp(ast->left->redirection_file, "\0") == 0)
	// 		{
	// 				symbol = "newline";
	// 				ft_fprintf(STDERR_FILENO,
	// 					"minishell: syntax error near unexpected token `%s'\n",symbol);
	// 				*exit_status = 258;
	// 				return (1);
	// 		}
	// 	}
	// }
	return (0);
}

void	execute_ast(t_ast *ast, int *exit_status)
{
	int	saved_stdin;
	int	saved_stdout;

	if (syntax_check(ast, exit_status))
		return ;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	execute_pipeline(ast, exit_status);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return ;
}

void	close_pipes(int *pipe_fds, int num_pipes)
{
	int	i;

	i = 0;
	while (i < 2 * num_pipes)
	{
		close(pipe_fds[i]);
		i++;
	}
}
