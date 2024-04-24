/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:27:29 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/24 08:54:56 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

typedef struct s_envhelper
{
	char	*name;
	char	*value;
	char	*env_str;
	char	**new_env;
}			t_envhelper;

char		**ft_realloc_env(char **envp, char *new_var);
void		ft_free_env(char **env);
char		**ft_copy_env(char **envp);
int			ft_setenv(const char *name, const char *value, int overwrite,
				char ***envp);
int			unset_env_var(char *var, char ***envp);
int			ft_putenv(char *string, char ***envp);
char		*ft_getenv(const char *name, char **envp);

#endif