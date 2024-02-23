/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:58:44 by mleibeng          #+#    #+#             */
/*   Updated: 2024/02/22 11:12:22 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

void	update_pwd(t_key **head, const char *old_dir);

void	change_dir(const char *path)
{
	if (chdir(path))
		perror("error changing directory");
}

// this one is just in case :))
// we will change it and put it in another file wehre we can handle all the errors
// are we allowed to use fprintf???????
void print_error(const char *path, const char *message)
{
    fprintf(stderr, "Error accessing %s: %s\n", path, message);
}
/*--------------------------some details what i did here :))---------------------------------*/
// changed the working dir of the current process
// head is the pointer to t_key struct
// path is used to change the working directory
// exit success or failure (we could use also bool) depends how you prefer,
		// i think it would work with bool aso , but its not so importnat 


int	cd(t_key **head, const char *path)
{
	t_key	*key;
	char	*old_dir;

	if (!path || !(*path))
	{
		key = find_env_key(*head, "HOME");
		if (!key)
		{
			print_error("HOME", "not set");				// i don't know if we need all these checks and prints
			return (EXIT_FAILURE);						// but we will figure out 
		}												// i just checked them for us to understand in case it failes where to look
		path = key->value;
		if (!path)
		{
			print_error("HOME", "empty");
			return (EXIT_FAILURE);
		}
	}
	old_dir = getcwd(NULL, 0);
	if (old_dir == NULL)
	{
		print_error("getcwd", strerror(errno));
		return (EXIT_FAILURE);
	}
	if (chdir(path) != EXIT_SUCCESS)
	{
		print_error(path, strerror(errno));
		free(old_dir);
		return (EXIT_FAILURE);
	}
	update_pwd(head, old_dir);
	free(old_dir);
	return (EXIT_SUCCESS);
}

/*--------------------------some details what i did here :))---------------------------------*/
// updates the path and old path 
// we need the old directory to set the OLDPWD to

void	update_pwd(t_key **head, const char *old_dir)
{
	char	*new_dir;
	t_key	*pwd_env;				// we could use them like this or ..... we can put them in the struct 
	t_key	*old_pwd_env;			// i have no idea how is the best way, but, i bet you do ;)

	if (!old_dir)
		return ;					//to to handle potential failures gracefully :)
	old_pwd_env = find_env_key(*head, "OLDPWD");
	if (old_pwd_env)
	{
		free(old_pwd_env->value);
		free(old_pwd_env->string);								// we need a good name for the full string of the path, i just called it string
		old_pwd_env->value = ft_strdup(old_dir);				// declared in t_key struct
		old_pwd_env->string = ft_strjoin("OLDPWD=", old_dir);
	}
	new_dir = getcwd(NULL, 0);
	if (!new_dir)
		return ;
	pwd_env = find_env_key(*head, "PWD");
	if (pwd_env)
	{
		free(pwd_env->value);
		free(pwd_env->string);
		pwd_env->value = ft_strdup(new_dir);
		pwd_env->string = ft_strjoin("PWD=", new_dir);
	}
	free(new_dir);
}
/*--------------------------some details what i did here :))---------------------------------*/

// here it has to find the node who has the given key
// the key is the one we need to search
//and it has to return the node which has the key, and otherwise if it's not found returns NULL 

t_key *find_env_key(t_key *head, const char *key_name)
{
	t_key	*current_key;

	if (!key_name)				// check to avoid seg faults
		return (NULL);	
	current_key = head;
	while (current_key != NULL)
	{
		if (strcmp(current_key->key, key_name) == 0)
			return (current_key);
		current_key = current_key->next;
	}
	return (NULL);
}


/// is there is a better way how i can write the comments, ley me know 