/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:27:11 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/02 05:00:17 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include "parser.h"
# include "builtins.h"
# include "redirection.h"

int     execute_ast(t_ast *ast, int *exit_status);
int     execute_builtin(t_ast *ast, int *exit_status);

#endif