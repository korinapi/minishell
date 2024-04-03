/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 05:16:09 by marvinleibe       #+#    #+#             */
/*   Updated: 2024/04/01 22:31:54 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"

int	execute_env(void)
{
	int i;
	i = 0;
	while (environ[i])
	{
		ft_putendl_fd(environ[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}