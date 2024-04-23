/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:34:52 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/23 21:34:22 by mleibeng         ###   ########.fr       */
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

int	find_env_var(char *name)
{
	int	i;

	i = 0;
	while (environ[i])
	{
		if (!ft_strncmp(environ[i], name, ft_strlen(name))
			&& environ[i][ft_strlen(name)] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	update_env_var(char *name, char *value)
{
	int		index;
	int		len;
	char	*new_str;

	index = find_env_var(name);
	if (index == -1)
		return (-1);
	len = ft_strlen(name) + ft_strlen(value) + 2;
	new_str = (char *)malloc(len);
	if (!new_str)
		return (-1);
	ft_snprintf(new_str, len, "%s=%s", name, value);
	free(environ[index]);
	environ[index] = new_str;
	return (0);
}

int	add_env_var(char *name, char *value)
{
	int		count;
	int		len;
	char	*new_str;
	char	**new_env;

	count = 0;
	while (environ[count])
		count++;
	len = ft_strlen(name) + ft_strlen(value) + 2;
	new_str = malloc(len);
	if (!new_str)
		return (-1);
	ft_snprintf(new_str, len, "%s=%s", name, value);
	new_env = ft_realloc_env(environ, new_str);
	if (!environ)
	{
		free(new_str);
		return (-1);
	}
	free(new_str);
	environ = new_env;
	return (0);
}

int	ft_putenv(char *string)
{
	int		index;
	char	*name;
	char	*value;

	if (parse_env_string(string, &name, &value) != 0)
		return (-1);
	index = find_env_var(name);
	if (index == -1)
		return (add_env_var(name, value));
	else
		return (update_env_var(name, value));
}
