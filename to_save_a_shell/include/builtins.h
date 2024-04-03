/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvinleibenguth <marvinleibenguth@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:30:40 by mleibeng          #+#    #+#             */
/*   Updated: 2024/03/30 23:03:24 by marvinleibe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

#include "parser.h"

void    exit_shell(int *exit_status);
int     execute_exit(char **args, int *exit_status);
int     execute_echo(t_ast *args, int *exit_status);
int     execute_env();
int     execute_export(t_ast *args);
int     execute_unset(t_ast *args);
int     execute_cd(t_ast *args);
int     execute_pwd(void);

#endif