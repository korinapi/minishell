/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvinleibenguth <marvinleibenguth@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:29:28 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/25 12:56:02 by marvinleibe      ###   ########.fr       */
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

int	execute_external(char **args, int *exit_status, char ***envp)
{
	pid_t	pid;
	char	*command_path;

	rl_catch_signals = 1;
	if (!*args)
		return (0);
	command_path = get_command_path(args[0], *envp);
	if (!command_path)
	{
		ft_error("minishell", "command not found", args[0]);
		*exit_status = 127;
		return (free(command_path), *exit_status);
	}
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	else if (pid == 0)
	{
		execute_command_from_path(args, command_path, exit_status, *envp);
		return (1);
	}
	else
		*exit_status = wait_and_update_status(pid);
	rl_catch_signals = 0;
	return (free(command_path), *exit_status);
}


void	execute_simple_command(t_ast *node, int *exit_status, char ***envp)
{
	char	**args;
	int		i;
	t_ast	*current_node;
	t_ast	*root;

	root = node;
	current_node = node->left;
	
	if (handle_redirection(current_node, exit_status))
		return ;
	args = ft_calloc(ast_count_nodes(current_node) + 1, sizeof(char *));
	if (!args)
		return ;
	i = 0;
	while (current_node)
	{
		if (current_node->type != AST_WHITESPACE
			&& current_node->type != AST_REDIRECTION)
			args[i++] = current_node->data;
		current_node = current_node->right;
	}
	args[i] = NULL;
	if (is_builtin(args[0]))
	{
		free(args);
		*exit_status = execute_builtin(node, root, exit_status, envp);
		return ;
	}
	else
		*exit_status = execute_external(args, exit_status, envp);
	free(args);
}

// int	syntax_check(t_ast *last, int *exit_status)
// {
// 	//printf("Node: %s     node type: %u\n", last->data, last->type);
// 	if (last->type == 0 && !last->left->left)
// 	{
// 		ft_fprintf(STDERR_FILENO,
// 					"minishell: syntax error near unexpected token `|'\n");
// 				*exit_status = 258;
// 				return (1);
// 	}
// 	else if ((last->type == 1) && !last->left->left && !last->redirection_file)
// 	{
// 		ft_fprintf(STDERR_FILENO,
// 					"minishell: syntax error near unexpected token `newline'\n");
// 				*exit_status = 258;
// 				return (1);
// 	}
// 	return (0);
// }
int	syntax_check_extra(t_ast *ast, int *exit_status)
{
	static char	*symbol;

	if ((ast->left->redirection_mode == REDIR_IN
			|| ast->left->redirection_mode == REDIR_OUT
			|| ast->left->redirection_mode == REDIR_OUT_APPEND
			|| ast->left->redirection_mode == REDIR_HEREDOC))
	{
		if (ft_strcmp(ast->left->redirection_file, "\0") == 0)
		{
			symbol = "newline";
			ft_fprintf(STDERR_FILENO,
				"minishell: syntax error near unexpected token `%s'\n", symbol);
			*exit_status = 258;
			return (1);
		}
	}
	return (0);
}

int	syntax_check(t_ast *ast, int *exit_status)
{
	if (!ast->left)
	{
		*exit_status = 127;
		return (1);
	}
	if (ast->type == AST_PIPELINE && !ast->left->left)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n");
		*exit_status = 258;
		return (1);
	}
	if (ast->type == AST_PIPELINE && (ast->left->type == AST_SIMPLE_COMMAND)
		&& (ast->left->left && ast->left->left->type == AST_REDIRECTION
			&& (ast->left->left->redirection_mode == REDIR_IN
				|| ast->left->left->redirection_mode == REDIR_HEREDOC)
			&& ft_strcmp(ast->left->left->redirection_file, "\0") == 0))
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n");
		*exit_status = 258;
		return (1);
	}
	if (ast->type == AST_SIMPLE_COMMAND)
	{
		if (syntax_check_extra(ast, exit_status) == 1)
			return (1);
	}
	return (0);
}
// t_ast	*get_last_node(t_ast *node)
// {
// 	if (!node)
// 		return (NULL);
// 	get_last_node(node->left);
// 	get_last_node(node->right);
// 	//printf("Node: %s     node type: %u       redir_mode: %d\n", node->data,
// node->type, node->redirection_mode);
// 	return (node);
// }

void	execute_ast(t_ast *ast, int *exit_status, char ***envp)
{
	int	saved_stdin;
	int	saved_stdout;

	// t_ast *last;
	// last = get_last_node(ast);
	// printf("Last Node: %s     node type: %u\n", last->data, last->type);
	if (syntax_check(ast, exit_status))
		return ;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	execute_pipeline(ast, exit_status, envp);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return ;
}
