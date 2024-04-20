/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:30:40 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/19 10:12:56 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "parser.h"

void	exit_shell(t_ast *ast, char **args, int exit_status);
int		execute_exit(t_ast *ast, int *exit_status);
int		execute_echo(t_ast *ast, int *exit_status);
int		execute_env(void);
int		execute_export(t_ast *ast);
int		execute_unset(t_ast *ast);
int		execute_cd(t_ast *ast);
int		execute_pwd(void);
void	bubble_sort(int n);
int		valid_check(t_ast *arg, char *var_name);
void	set_pwd(void);
void	process_double_quoted_str(const char *quoted_str, char *exit_stat);

#endif