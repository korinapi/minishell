/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 05:00:31 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/01 22:31:53 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "errors.h"
#include "executor.h"
#include "signals.h"

void	exit_shell(int *exit_status)
{
	ft_free_env(environ);
	rl_clear_history();
	printf("exit\n");
	exit(*exit_status);
}

int	execute_exit(char **args, int *exit_status)
{
	int	exit_value;

	if (!args[1])
		exit_shell(exit_status);
	exit_value = ft_atoi(args[1]);
	if (exit_value < 0 || exit_value > 255)
	{
		ft_error("minishell", "exit", "numeric argument required");
		*exit_status = 2;
		exit_shell(exit_status);
	}
	else if (args[2])
	{
		ft_error("minishell", "exit", "too many arguments");
		return (1);
	}
	*exit_status = exit_value;
	exit_shell(exit_status);
	return (0);
}
