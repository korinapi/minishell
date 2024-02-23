/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 15:16:32 by cpuiu             #+#    #+#             */
/*   Updated: 2024/01/24 13:56:22 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include "../Libft/libft.h"
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

int				get_next_line(int const fd, char **line);
void			shift_left(char *str, int index);
int				process_line(int i, char **line, char *result, char *rest);
int				concatenate_line(char **line, char *rest, int *endline);
int				read_line(int fd, char **line, int endline, char *rest);

#endif
