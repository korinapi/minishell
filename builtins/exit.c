/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:59:29 by mleibeng          #+#    #+#             */
/*   Updated: 2024/02/13 22:34:29 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"


// probs needs to do alot of other shit as well
int exit_func(t_data *data)
{
	if (data->input != NULL)
		free(data->input);
	rl_clear_history();
	exit(data->exitcode);
}