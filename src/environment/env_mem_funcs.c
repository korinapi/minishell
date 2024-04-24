/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_mem_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 08:49:41 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/24 03:45:16 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "free.h"
#include "minishell.h"
#include "utilities.h"

char	**ft_realloc_env(char **envp, char *new_var)
{
	char	**new_env;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (NULL);
	i = -1;
	while (envp[++i])
		new_env[i] = envp[i];
	new_env[i] = ft_strdup(new_var);
	if (!new_env[i])
	{
		free(new_var);
		free(new_env);
		return (NULL);
	}
	new_env[i + 1] = NULL;
	free(envp);
	return (new_env);
}

char	**ft_copy_env(char **envp)
{
	char	**env;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	env = ft_calloc(i + 1, sizeof(char *));
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
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

char	*ft_getenv(const char *name, char **envp)
{
	int	len;
	int	i;

	i = 0;
	len = ft_strlen(name);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	ft_setenv(const char *name, const char *value, int overwrite, char ***envp)
{
	char	*env_str;
	int		len;
	int		status;
	char	*existing;

	existing = ft_getenv(name, *envp);
	if (existing && !overwrite)
		return (0);
	len = ft_strlen(name) + ft_strlen(value) + 2;
	env_str = (char *)malloc(len * sizeof(char));
	if (!env_str)
		return (-1);
	ft_snprintf(env_str, len, "%s=%s", name, value);
	status = ft_putenv(env_str, envp);
	free(env_str);
	return (status);
}

int	unset_env_var(char *var, char ***envp)
{
	int	var_len;
	int	i;
	int	j;

	var_len = ft_strlen(var);
	i = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], var, var_len) && (*envp)[i][var_len] == '=')
		{
			free((*envp)[i]);
			j = i;
			while ((*envp)[j])
			{
				(*envp)[j] = (*envp)[j + 1];
				j++;
			}
			return (0);
		}
		i++;
	}
	return (1);
}
