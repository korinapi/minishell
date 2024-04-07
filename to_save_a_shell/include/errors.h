/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 19:00:09 by marvinleibe       #+#    #+#             */
/*   Updated: 2024/04/07 02:50:43 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

void	ft_error(char *program_name, char *operation, char *error_message);
void	ft_error2(char *program_name, char *operation, char *arg,
			char *error_message);

#endif