/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:27:59 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/04 20:18:35 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITIES_H
# define UTILITIES_H

# include "parser.h"

void	print_ast(t_ast *node, int level, char *node_type);
char	**ast_to_arg(t_ast *node, int *exit_status);
int		is_valid(const char *str);

#endif