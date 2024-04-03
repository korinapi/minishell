/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvinleibenguth <marvinleibenguth@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:27:29 by mleibeng          #+#    #+#             */
/*   Updated: 2024/03/30 23:31:12 by marvinleibe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

#include "minishell.h"
#include "parser.h"

char    **ft_realloc_env(char *new_var);
void    ft_free_env(char **env);
char    **ft_copy_env();
int     ft_setenv(const char *name, const char *value, int overwrite);
int     unset_env_var(char *var);
int     ft_putenv(char *string);

#endif