/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:27:29 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/03 17:48:35 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

char	**ft_realloc_env(char *new_var);
void	ft_free_env(char **env);
char	**ft_copy_env(void);
int		ft_setenv(const char *name, const char *value, int overwrite);
int		unset_env_var(char *var);
int		ft_putenv(char *string);

#endif