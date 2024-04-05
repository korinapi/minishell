/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:30:09 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/05 18:45:25 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include "minishell.h"
#include "parser.h"

int	execute_builtin(t_ast *ast, int *exit_status)
{
	while (ast->left)
		ast = ast->left;
	if (!ft_strcmp(ast->data, "echo"))
		return (execute_echo(ast, exit_status));
	else if (!ft_strcmp(ast->data, "cd"))
		return (execute_cd(ast));
	else if (!ft_strcmp(ast->data, "pwd"))
		return (execute_pwd());
	else if (!ft_strcmp(ast->data, "unset"))
		return (execute_unset(ast));
	else if (!ft_strcmp(ast->data, "export"))
		return (execute_export(ast));
	else if (!ft_strcmp(ast->data, "env"))
		return (execute_env());
	else if (!ft_strcmp(ast->data, "exit"))
		return (execute_exit(ast, exit_status));
	else
		return (1);
}
