/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:34:52 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/24 03:43:25 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "minishell.h"
#include "utilities.h"

int	parse_env_string(char *string, char **name, char **value)
{
	char	*eq_pos;

	eq_pos = ft_strchr(string, '=');
	if (!eq_pos)
	{
		*name = string;
		*value = "";
	}
	else
	{
		*eq_pos = '\0';
		*name = string;
		*value = eq_pos + 1;
	}
	return (0);
}

int	find_env_var(char *name, char ***envp)
{
	int	i;

	i = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], name, ft_strlen(name))
			&& (*envp)[i][ft_strlen(name)] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	update_env_var(char *name, char *value, char ***envp)
{
	int		index;
	int		len;
	char	*new_str;

	index = find_env_var(name, envp);
	if (index == -1)
		return (-1);
	len = ft_strlen(name) + ft_strlen(value) + 2;
	new_str = (char *)malloc(len);
	if (!new_str)
		return (-1);
	ft_snprintf(new_str, len, "%s=%s", name, value);
	free((*envp)[index]);
	(*envp)[index] = new_str;
	return (0);
}

int	add_env_var(char *name, char *value, char ***envp)
{
	int		count;
	int		len;
	char	*new_str;
	char	**new_env;

	count = 0;
	while ((*envp)[count])
		count++;
	len = ft_strlen(name) + ft_strlen(value) + 2;
	new_str = malloc(len);
	if (!new_str)
		return (-1);
	ft_snprintf(new_str, len, "%s=%s", name, value);
	new_env = ft_realloc_env(*envp, new_str);
	if (!new_env)
	{
		free(new_str);
		return (-1);
	}
	free(new_str);
	*envp = new_env;
	return (0);
}

int	ft_putenv(char *string, char ***envp)
{
	int		index;
	char	*name;
	char	*value;

	if (parse_env_string(string, &name, &value) != 0)
		return (-1);
	index = find_env_var(name, envp);
	if (index == -1)
		return (add_env_var(name, value, envp));
	else
		return (update_env_var(name, value, envp));
}
