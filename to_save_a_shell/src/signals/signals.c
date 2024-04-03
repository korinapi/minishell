/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:35:06 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/03 14:10:45 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "executor.h"
#include "signals.h"

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
		g_sigint_received = 1;
	}
	else if (signo == SIGQUIT)
	{
	}
	(void)return_value;
}

void	setup_signals(void)
{
	struct sigaction sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
