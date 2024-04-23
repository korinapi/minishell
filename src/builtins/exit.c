/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 05:00:31 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/23 18:13:23 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "errors.h"
#include "executor.h"
#include "free.h"
#include "minishell.h"
#include "signals.h"
#include "utilities.h"

void	exit_shell(t_ast *ast, char **args, int exit_status)
{
	if (args)
		ft_free_split(args);
	free_ast(ast);
	ft_free_env(environ);
	rl_clear_history();
	exit(exit_status);
}

int	numeric_check(char **args, char *end, int *exit_status)
{
	if (*end != '\0')
	{
		printf("exit\n");
		ft_error2("minishell", "exit", args[1], "numeric argument required");
		*exit_status = 255;
		return (1);
	}
	return (0);
}

int	execute_exit(t_ast *ast, int *exit_status)
{
	char	**args;
	char	*end;
	int		exit_value;

	args = ast_to_arg(ast, exit_status);
	if (!args[1])
	{
		printf("exit\n");
		exit_shell(ast, args, *exit_status);
	}
	exit_value = ft_strtoi(args[1], &end);
	if (numeric_check(args, end, exit_status))
		exit_shell(ast, args, *exit_status);
	if (args[2])
	{
		printf("exit\n");
		ft_error("minishell", "exit", "too many arguments");
		ft_free_split(args);
		return (1);
	}
	*exit_status = exit_value % 256;
	printf("exit\n");
	exit_shell(ast, args, *exit_status);
	return (0);
}
