/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:34:52 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/03 17:51:24 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "environment.h"
#include "utilities.h"

char	**ft_realloc_env(char *new_var)
{
	char	**new_env;

	int i, size;
	size = 0;
	while (environ[size])
		size++;
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (environ[i])
	{
		new_env[i] = malloc(ft_strlen(environ[i]) + 1);
		if (!new_env[i])
			return (NULL);
		ft_strcpy(new_env[i], environ[i]);
		i++;
	}
	new_env[i] = malloc(ft_strlen(new_var) + 1);
	if (!new_env[i])
		return (NULL);
	ft_strcpy(new_env[i], new_var);
	new_env[i + 1] = NULL;
	return (new_env);
}

char	**ft_copy_env(void)
{
	char	**env;
	int		i;

	i = 0;
	while (environ[i])
		i++;
	env = ft_calloc(i + 1, sizeof(char *));
	if (!env)
		return (NULL);
	i = 0;
	while (environ[i])
	{
		env[i] = ft_strdup(environ[i]);
		if (!env[i])
		{
			ft_free_env(env);
			return (NULL);
		}
		i++;
	}
	env[i] = NULL;
	return (env);
}

int	ft_setenv(const char *name, const char *value, int overwrite)
{
	char	*env_str;
	int		len;
	char	*existing;

	existing = getenv(name);
	if (existing && !overwrite)
	{
		return (0);
	}
	len = ft_strlen(name) + ft_strlen(value) + 2;
	env_str = (char *)malloc(len);
	if (!env_str)
	{
		return (-1);
	}
	ft_snprintf(env_str, len, "%s=%s", name, value);
	return (putenv(env_str));
}

int	ft_putenv(char *string)
{
	char	*name;
	char	*value;
	char	*env_str;
	size_t	count;
	size_t	i;
	int		len;
	char	**new_env;

	value = ft_strchr(string, '=');
	if (!value)
	{
		return (-1);
	}
	*value = '\0';
	value++;
	name = string;
	for (count = 0; environ[count]; count++)
		;
	new_env = (char **)ft_realloc(environ, (count + 2) * sizeof(char *));
	if (!new_env)
	{
		return (-1);
	}
	environ = new_env;
	len = ft_strlen(name) + ft_strlen(value) + 2;
	env_str = (char *)malloc(len);
	if (!env_str)
	{
		return (-1);
	}
	ft_snprintf(env_str, len, "%s=%s", name, value);
	for (i = 0; environ[i]; i++)
	{
		if (ft_strncmp(environ[i], name, ft_strlen(name)) == 0
			&& environ[i][ft_strlen(name)] == '=')
		{
			free(environ[i]);
			environ[i] = env_str;
			return (0);
		}
	}
	environ[count] = env_str;
	environ[count + 1] = NULL;
	return (0);
}

int	unset_env_var(char *var)
{
	int var_len;
	int i;
	int j;
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