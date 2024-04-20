/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 05:16:09 by marvinleibe       #+#    #+#             */
/*   Updated: 2024/04/19 12:40:23 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "minishell.h"

int	execute_env(void)
{
	int	i;

	i = 0;
	while (environ[i])
	{
		ft_putendl_fd(environ[i], 1);
		i++;
	}
	return (0);
}
