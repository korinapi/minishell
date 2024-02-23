/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:59:22 by mleibeng          #+#    #+#             */
/*   Updated: 2024/02/23 13:22:23 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"


// no fkin idea, just like the other 2


//// this one should print all the environmental vars provided in the struct, in the std output, right ?

/// @brief  
/// @param head 
/// @return 

int env_func(t_key *head)
{
	t_key	*buffer;

	buffer = head;
	while (buffer)
	{
		if (buffer->value)
			fprintf(stderr, "%s\n", buffer->string);
		buffer = buffer->next;
	}
	return (EXIT_SUCCESS);
}
