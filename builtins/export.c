/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:59:04 by mleibeng          #+#    #+#             */
/*   Updated: 2024/02/23 13:27:07 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

// no fkin idea rn!


/// @brief 
/// @param data 


// should we do them void or int ????

int	export_func(t_key **head, char **str)
{
	int	i;

	i = 0;
	if (!str[0])
	{
		put_error();
		return (EXIT_FAILURE);
	}
	while (str[i])
	{
		if (!is_key_valid(str[i]))                              // have to check this one 
			print_error("export", str[i], IDENTIFIER);			// also the funtion used previous 
		else
			process_string(head, str[i]);						// a function to process -> the main one for exporting 
		i++;
	}
	return (EXIT_SUCCESS);
}