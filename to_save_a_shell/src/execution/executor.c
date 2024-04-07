/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:29:28 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/07 08:16:05 by mleibeng         ###   ########.fr       */
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

char	*find_command_path(char *command, char **paths)
{
	char	*full_path;
	int		i;

	if (ft_strchr(command, '/') != NULL)
	{
		if (!access(command, X_OK))
			return (ft_strdup(command));
		else
			return (NULL);
	}
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin_free(full_path, command, 1);
		if (!access(full_path, X_OK))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

// ft_free_split would be necessary
// here if I didnt already free the ast elements later on...
// Since i do not strdup the data in the nodes,
// I dont need to free.
int	execute_external(t_ast *ast, int *exit_status)
{
	pid_t			pid;
	int				status;
	int				i;
	t_exechelper	exechelper;

	exechelper.args = ft_calloc(ast_count_nodes(ast->left) + 1, sizeof(char *));
	i = 0;
	exechelper.current_node = ast->left;
	while (exechelper.current_node)
	{
		if (exechelper.current_node->type != AST_WHITESPACE)
			exechelper.args[i++] = exechelper.current_node->data;
		exechelper.current_node = exechelper.current_node->right;
	}
	exechelper.args[i] = NULL;
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	else if (pid == 0)
	{
		exechelper.path_env = getenv("PATH");
		exechelper.paths = ft_split(exechelper.path_env, ':');
		exechelper.command_path = find_command_path(exechelper.args[0],
				exechelper.paths);
		if (exechelper.command_path == NULL)
		{
			ft_putstr_fd("Command not found: ", STDERR_FILENO);
			ft_putstr_fd(exechelper.args[0], STDERR_FILENO);
			ft_putchar_fd('\n', STDERR_FILENO);
			exit(127);
		}
		execve(exechelper.command_path, exechelper.args, NULL);
		ft_fprintf(STDERR_FILENO, "execve failed: %s\n", strerror(errno));
		exit(1);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			*exit_status = WEXITSTATUS(status);
		else
			*exit_status = 1;
	}
	return (*exit_status);
}

void	execute_simple_command(t_ast *node, int *exit_status)
void	execute_simple_command(t_ast *node, int *exit_status)
{
	if (is_builtin(node->left->data))
		*exit_status = execute_builtin(node, exit_status);
	else
		*exit_status = execute_external(node, exit_status);
}

int	syntax_check(t_ast *ast, int *exit_status)
{
	if (ast->type == AST_PIPELINE && !ast->left->right)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n");
		*exit_status = 258;
		return (1);
	}
	return (0);
}

void	execute_ast(t_ast *ast, int *exit_status)
{
	int	saved_stdin;
	int	saved_stdout;

	if (syntax_check(ast, exit_status))
		return ;
	if (ast->type == AST_SIMPLE_COMMAND)
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		handle_redirection(ast);
		execute_simple_command(ast, exit_status);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return ;
	}
	else if (ast->type == AST_PIPELINE)
	{
		execute_pipeline(ast, exit_status);
		return ;
	}
	return ;
}
