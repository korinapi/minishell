/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:35:06 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/26 15:08:31 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "executor.h"
#include "signals.h"
#include "minishell.h"

volatile sig_atomic_t	g_sigint_received = 0;

void	signal_handler(int signo)
{
	int	return_value;

	if (signo == SIGINT)
	{
		rl_on_new_line();
		return_value = write(STDERR_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_redisplay();
		rl_point = 0;
	}
	(void)return_value;
}

void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	signal_handler_child(int signo)
{
	int	return_value;

	if (signo == SIGINT)
	{
		rl_on_new_line();
		return_value = write(STDERR_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_point = 0;
	}
	(void)return_value;
}

void	setup_child_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_handler_child;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGINT);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
