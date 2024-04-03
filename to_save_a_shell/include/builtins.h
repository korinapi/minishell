/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:30:40 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/03 17:50:41 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

#include "parser.h"

void	exit_shell(int *exit_status);
int		execute_exit(t_ast *ast, int *exit_status);
int		execute_echo(t_ast *ast, int *exit_status);
int		execute_env(void);
int		execute_export(t_ast *ast);
int		execute_unset(t_ast *ast);
int		execute_cd(t_ast *ast);
int		execute_pwd(void);

#endif