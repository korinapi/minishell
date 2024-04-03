/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:30:09 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/01 23:15:53 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	execute_builtin(t_ast *ast, int *exit_status)
{
	while (ast->left)
		ast = ast->left;
	if (ft_strcmp(ast->data, "echo") == 0)
		return (execute_echo(ast, exit_status));
	else if (ft_strcmp(ast->data, "cd") == 0)
		return (execute_cd(ast));
	else if (ft_strcmp(ast->data, "pwd") == 0)
		return (execute_pwd());
	else if (ft_strcmp(ast->data, "unset") == 0)
		return (execute_unset(ast));
	else if (ft_strcmp(ast->data, "export") == 0)
		return (execute_export(ast));
	else if (ft_strcmp(ast->data, "env") == 0)
		return (execute_env());
	else
		return (1);
}