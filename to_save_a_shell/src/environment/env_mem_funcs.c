/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_mem_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 08:49:41 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/07 10:33:03 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "minishell.h"
#include "utilities.h"

void	ft_free_env_split(char **new_env, int i)
{
	int	j;

	j = 0;
	while (j < i)
		free(new_env[j++]);
	free(new_env);
	return ;
}

char	**ft_realloc_env(char **environ, char *new_var)
{
	char	**new_env;
	int		i;
	int		size;

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
			return (ft_free_env_split(new_env, i), NULL);
		ft_strcpy(new_env[i], environ[i]);
		i++;
	}
	new_env[i] = malloc(ft_strlen(new_var) + 1);
	if (!new_env[i])
		return (ft_free_env_split(new_env, i), NULL);
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
		return (0);
	len = ft_strlen(name) + ft_strlen(value) + 2;
	env_str = (char *)malloc(len);
	if (!env_str)
		return (-1);
	ft_snprintf(env_str, len, "%s=%s", name, value);
	return (ft_putenv(env_str));
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