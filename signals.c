/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 02:34:44 by mleibeng          #+#    #+#             */
/*   Updated: 2024/02/06 00:51:10 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		interrupt_process = 1;
		write(STDOUT_FILENO, "\n", 1);
	}
}

void	kill_children(int child_pid)
{
	if (child_pid > 0)
	{
		kill(child_pid, SIGTERM);
		waitpid(child_pid, NULL, 0);
	}
	interrupt_process = 0;
}

void	init_signals(void)
{
	struct sigaction sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("couldn't handle Ctrl-C");
		exit(EXIT_FAILURE);
	}
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("couldn't handle Ctrl-'\\'");
		exit(EXIT_FAILURE);
	}
}