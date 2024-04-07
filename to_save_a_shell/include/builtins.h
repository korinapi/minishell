/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:30:40 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/07 06:30:08 by mleibeng         ###   ########.fr       */
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
int	valid_check(t_ast *arg, char *var_name);
void	bubble_sort(int n);
void	swap_strings(char **a, char **b);
int	compare_strings(const char *a, const char *b);

#endif