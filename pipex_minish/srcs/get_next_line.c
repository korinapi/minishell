/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:34:54 by cpuiu             #+#    #+#             */
/*   Updated: 2024/02/06 21:12:28 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/get_next_line.h"
#include "../include/pipex.h"

void	shift_left(char *str, int index);
int		process_line(int i, char **line, char *result, char *rest);
int		concatenate_line(char **line, char *rest, int *endline);
int		read_line(int fd, char **line, int endline, char *rest);

void	shift_left(char *str, int index)
{
	int	i;

	i = index;
	while (str[i])
	{
		str[i - index] = str[i];
		i++;
	}
	str[i - index] = '\0';
}

int	process_line(int i, char **line, char *result, char *rest)
{
	*line = ft_strsub(result, 0, i);
	if (!(*line))
		return (1);
	i = 0;
	while (rest[i] != '\n')
		i++;
	shift_left(rest, i + 1);
	ft_strdel(&result);
	return (0);
}

int	concatenate_line(char **line, char *rest, int *endline)
{
	char	*result;
	int		i;

	result = ft_strjoin(*line, rest);
	if (!result)
		return (1);
	ft_strdel(line);
	i = 0;
	while (result[i] && result[i] != '\n')
		i++;
	if (result[i] == '\n')
	{
		if (process_line(i, line, result, rest) == 1)
			return (1);
		*endline = 1;
	}
	else
	{
		*line = ft_strdup(result);
		if (!(*line))
			return (1);
		ft_strdel(&result);
		ft_memset(rest, 0, BUFFER_SIZE);
	}
	return (0);
}

int	read_line(int fd, char **line, int endline, char *rest)
{
	int	ret;

	ret = 1;
	if (!endline)
	{
		ft_memset(rest, 0, BUFFER_SIZE);
		while (!endline && ret > 0)
		{
			ret = read(fd, rest, BUFFER_SIZE);
			if (concatenate_line(line, rest, &endline) == 1)
				return (-1);
		}
		if (ret == -1)
			return (ret);
	}
	if (!ft_strcmp(*line, "") && ret == 0)
		return (0);
	else
		return (1);
}

int	get_next_line(int const fd, char **line)
{
	static char		*rest = NULL;
	int				i;
	int				endline;

	if (rest == NULL)
	{
		rest = ft_strnew(BUFFER_SIZE);
		if (!rest)
			return (-1);
	}
	if (!line)
		return (-1);
	endline = 0;
	i = 0;
	while (rest[i] && rest[i] != '\n')
		i++;
	if (rest[i] == '\n')
		endline = 1;
	*line = ft_strndup(rest, i);
	if (!(*line))
		return (-1);
	shift_left(rest, i + 1);
	return (read_line(fd, line, endline, rest));
}
