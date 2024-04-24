/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:30:40 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/24 03:29:13 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "parser.h"

void	exit_shell(t_ast *ast, char **args, int exit_status, char ***envp);
int		execute_exit(t_ast *ast, t_ast *root, int *exit_status, char ***envp);
int		execute_echo(t_ast *ast, int *exit_status);
int		execute_env(char ***envp);
int		execute_export(t_ast *ast, char ***envp);
int		execute_unset(t_ast *ast, char ***envp);
int		execute_cd(t_ast *ast, char ***envp);
int		execute_pwd(void);
void	bubble_sort(char **envp, int n);
int		valid_check(t_ast *arg, char *var_name);
void	set_pwd(char ***envp);
void	process_double_quoted_str(const char *quoted_str, char *exit_stat);

#endif