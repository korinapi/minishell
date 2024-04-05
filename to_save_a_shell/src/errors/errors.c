/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:29:28 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/05 18:45:18 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"
#include "minishell.h"

void	ft_error(char *program_name, char *operation, char *error_message)
{
	ft_fprintf(STDOUT_FILENO, "%s: %s: %s\n", program_name, operation,
		error_message);
}

void	ft_error2(char *program_name, char *operation, char *arg,
		char *error_message)
{
	ft_fprintf(STDOUT_FILENO, "%s: %s: %s: %s\n", program_name, operation, arg,
		error_message);
}
