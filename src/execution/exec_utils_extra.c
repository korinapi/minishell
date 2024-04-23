/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_extra.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 20:43:53 by cpuiu             #+#    #+#             */
/*   Updated: 2024/04/23 20:45:10 by cpuiu            ###   ########.fr       */
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

void	close_pipes(int *pipe_fds, int num_pipes)
{
	int	i;

	i = 0;
	while (i < 2 * num_pipes)
	{
		close(pipe_fds[i]);
		i++;
	}
}
