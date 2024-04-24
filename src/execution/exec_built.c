/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:30:09 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/24 03:08:38 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include "minishell.h"
#include "parser.h"

int	execute_builtin(t_ast *ast, t_ast *root, int *exit_status, char ***envp)
{
	while (ast->left)
		ast = ast->left;
	while (ast && ast->type == AST_WHITESPACE)
		ast = ast->right;
	if (!ft_strcmp(ft_strtolower(ast->data), "echo"))
		return (execute_echo(ast, exit_status));
	else if (!ft_strcmp(ast->data, "cd"))
		return (execute_cd(ast, envp));
	else if (!ft_strcmp(ast->data, "pwd"))
		return (execute_pwd());
	else if (!ft_strcmp(ast->data, "unset"))
		return (execute_unset(ast, envp));
	else if (!ft_strcmp(ast->data, "export"))
		return (execute_export(ast, envp));
	else if (!ft_strcmp(ast->data, "env"))
		return (execute_env(envp));
	else if (!ft_strcmp(ast->data, "exit"))
		return (execute_exit(ast, root, exit_status, envp));
	else
		return (1);
}
