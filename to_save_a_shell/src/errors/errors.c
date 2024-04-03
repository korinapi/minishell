/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:29:28 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/01 22:34:27 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"

void	ft_error(char *program_name, char *operation, char *error_message)
{
	ft_fprintf(STDOUT_FILENO, "%s: %s: %s\n", program_name, operation, error_message);
}