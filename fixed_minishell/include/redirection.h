/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:31:42 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/18 19:57:00 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_H
# define REDIRECTION_H

int			handle_redirection(t_ast *node, int *exit_status);
void		execute_redirection(t_ast *node);
bool		has_redirection_nodes(t_ast *node);

#endif