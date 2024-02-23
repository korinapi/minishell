/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:58:35 by mleibeng          #+#    #+#             */
/*   Updated: 2024/02/04 00:44:26 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

//Needs a lot of work to also handle environment variables as well as multiple args!
int	echo_func(const char *string)
{
	int string_length;

	string_length = printf("%s", string);
	if (string_length < 0)
	{
		perror("error in echo function");
		return (-1);
	}
	return (0);
}
