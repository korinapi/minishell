/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvinleibenguth <marvinleibenguth@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 00:21:58 by marvinleibe       #+#    #+#             */
/*   Updated: 2024/03/29 00:28:00 by marvinleibe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FREE_H
#define FREE_H

#include "minishell.h"
#include "parser.h"

void	ft_free_env(char **env);
void	ft_free_split(char **split);
void	free_ast(t_ast *ast);

#endif