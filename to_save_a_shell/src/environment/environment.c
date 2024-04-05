/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:34:52 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/05 18:43:14 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "minishell.h"
#include "utilities.h"

int	replace_env_var_if_exists(const char *name, char *env_str)
{
	size_t	name_len;
	int		i;

	i = 0;
	name_len = ft_strlen(name);
	while (environ[i])
	{
		if (ft_strncmp(environ[i], name, name_len) == 0
			&& environ[i][name_len] == '=')
		{
			free(environ[i]);
			environ[i] = env_str;
			return (1);
		}
		i++;
	}
	return (0);
}

int	prepare_env_input(const char *string, char **name,  char **value)
{
	*value = ft_strchr(string, '=');
	if (!value)
		return (-1);
	**value = '\0';
	(*value)++;
	*name = (char *)string;
	return (0);
}

int	ft_putenv(char *string)
{
	char	*name;
	char	*value;
	char	*env_str;
	size_t	count;
	int		len;
	char	**new_env;

	if (prepare_env_input(string, &name, &value) != 0)
		return (-1);
	count = 0;
	while (environ[count])
		count++;
	new_env = (char **)ft_realloc(environ, (count + 2) * sizeof(char *));
	if (!new_env)
		return (-1);
	environ = new_env;
	len = ft_strlen(name) + ft_strlen(value) + 2;
	env_str = (char *)malloc(len);
	if (!env_str)
		return (-1);
	ft_snprintf(env_str, len, "%s=%s", name, value);
	if (replace_env_var_if_exists(name, env_str))
		return (0);
	environ[count] = env_str;
	environ[count + 1] = NULL;
	return (0);
}

int	unset_env_var(char *var)
{
	int	var_len;
	int	i;
	int	j;

	var_len = ft_strlen(var);
	i = 0;
	while (environ[i])
	{
		if (!ft_strncmp(environ[i], var, var_len) && environ[i][var_len] == '=')
		{
			free(environ[i]);
			j = i;
			while (environ[j])
			{
				environ[j] = environ[j + 1];
				j++;
			}
			return (0);
		}
		i++;
	}
	return (1);
}
