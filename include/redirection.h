/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:31:42 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/29 18:05:48 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_H
# define REDIRECTION_H

int		handle_redirection(t_ast *node, int *exit_status);
int		execute_redirection(t_ast *node);
bool	has_redirection_nodes(t_ast *node);
int		create_temp_file(char **tmp_file);
int		execute_heredoc_list(t_ast *heredoc_node);
int		execute_file_redirection_list(t_ast **file_node_ptr);
int		execute_file_redirection(t_ast *node);
void	handle_file_mode_redirection(t_ast *new_node, t_ast ***file_tail);
void	handle_heredoc_mode_redirection(t_ast *new_node, t_ast ***heredoc_tail);

#endif