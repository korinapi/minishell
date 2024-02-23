/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 19:59:44 by mleibeng          #+#    #+#             */
/*   Updated: 2024/02/22 06:03:45 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

/* Steps to take for minishell:
	1st. read the input ->done
	2nd. scan the input ->done
	3rd. Tokenize the input ->done
	4th. Check input for flags/options -> parse / syntax check the tokens ->only needs -n
	5th. variable expansion ex.: $ARG becomes "1 2 3 4" -> in progress
	6th. command execution using execve and pathes
	7th. error handle output (optional?)
	8th. handle environment vars
	9th. edit the command history -> done
	10th. implement signal handling ->done
*/

/* Done today
	Cleaned up some code blocks
	Local Variable assignment seems to work for now
	Started on var expansion -> initialized structure for norminette :(
		--> wrote code that searched for the occurence of $ in the token, then searched until the end of token to save as key
		--> parse hash then create a new token and overwrite the chars of the key with the value
		--> free the old token and assign the new one into the array (Doesn't work right now!!)
*/

/*To do
	Fix issues with uncertain path check for commands (sometimes returns null instead of the value on repeated calls)
	Var Expansion causes segmentation fault
	Search for Variables in environment once locally integrated variables return NULL (because there is no local var duh..)
*/

volatile sig_atomic_t interrupt_process = 0;

void	init_hashdata(t_data *data)
{
	data->hash_data = malloc(sizeof(t_hashdata));
	if (!data->hash_data)
	{
		perror("error in hash_malloc");
		return ;
	}
	data->hash_data->end = NULL;
	data->hash_data->key = NULL;
	data->hash_data->key_len = 0;
	data->hash_data->start = NULL;
	data->hash_data->var = NULL;
	data->hash_data->val = NULL;
}

void	init_data(t_data *data)
{
	data->envp = NULL;
	data->exitcode = 0;
	data->input = NULL;
	data->num_of_elements = 0;
	data->tokens = NULL;
	create_hash(data, INIT_HASH);
	init_hashdata(data);
}

int	main(int argc, char **argv)
{
	t_data data;

	(void) argv;
	if (argc != 1)
	{
		perror("Too many arguments for startup");
		return (-1);
	}
	init_data(&data);
	init_signals();
	while (1)
	{
		if (read_in("minishell> ", &data) == EOF)
			exit_func(&data);
	}
	exit_func(&data);
	return (0);
}