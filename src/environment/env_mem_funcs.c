/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_mem_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 08:49:41 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/22 10:56:27 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "minishell.h"
#include "utilities.h"

char	**ft_realloc_env(char **environ, char *new_var)
{
	char	**new_env;
	int		i;

	i = 0;
	while (environ[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (NULL);
	i = -1;
	while (environ[++i])
		new_env[i] = environ[i];
	new_env[i] = ft_strdup(new_var);
	if (!new_env[i])
	{
		free(new_env);
		return (NULL);
	}
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
	int		status;

	existing = getenv(name);
	if (existing && !overwrite)
		return (0);
	len = ft_strlen(name) + ft_strlen(value) + 2;
	env_str = (char *)malloc(len);
	if (!env_str)
		return (-1);
	ft_snprintf(env_str, len, "%s=%s", name, value);
	status = ft_putenv(env_str);
	if (status != 0)
        free(env_str);
	return (status);
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
