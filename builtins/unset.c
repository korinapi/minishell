/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:59:11 by mleibeng          #+#    #+#             */
/*   Updated: 2024/02/22 13:06:22 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

void	remove_key(char *key, t_key **head);
static bool	is_key_valid(const char *key);
// yeaa no idea as well!

// i noticed you don't have the simple ft_strcmp in your libft, we can add it if you agree :)

/*-------------------------------------------*/
//removes a var from key list
//it search a given key , if the key is found it frees the node and updates the list
// head -> pointer to the list
// keys -> the key to be removed
//returns the status if the key is valid or not
/*-------------------------------------------*/

int unset_func(t_key **head, char **keys)
{
	int	i;
	int	unset_status;
	
	if (!head || !keys)
	i = 1;
	unset_status = 0;
	while(keys[i])
	{
		if (!is_key_valid(keys[i]))
		{
			print_error_unset("unset", keys[i], IDENTIFIER); /// or something like this
			unset_status = 1;
		}
		else
		{
			remove_key(keys[i], head);
			unset_status = 1;
		}
		i++;
	}
	return (unset_status);
}

// key to be checked 
// false if there is an invalid key , true otherwise
// the key contains onlu alpha numeric or '_'

static bool	is_key_valid(const char *key)
{
	int	i;

	i = 0;
	if (!key)
		return (false);
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

// removing the key .....ughhhhhh
// key -> what we have to remove
// head -> the pointer
// should we make the int type and check the exit status also ????? :))) fuckkkkkkkk 
static void	remove_key(char *key, t_key **head)
{
	t_key	*buffer;
	t_key	*previous;
	
	if (!key || !head || !(*head))				//checking? no checking ? 
		return ;
	previous = NULL;
	buffer = *head;
	while (buffer)
	{
		if (strcmp(buffer->key, key) == 0)
		{
			if (!previous)
				*head = buffer->next;
			else
				previous->next = buffer->next;
			//previous->next = buffer->next;  // i am not 100 % if we should do this line , should we?
			free_envp_node(buffer);      /// we need this function where we will free everything
			return ;
		}
		previous = buffer;
		buffer = buffer->next;
	}
	// should we print an error here????? hmm
	// if the key wasn't found or whatever
}
