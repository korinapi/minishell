/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:07:03 by cpuiu             #+#    #+#             */
/*   Updated: 2023/12/21 18:36:22 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*buff;
	size_t	size;
	size_t	i;

	buff = (char *)s1;
	size = ft_strlen(s1);
	buff = malloc(size * sizeof(char) + 1);
	if (!buff)
		return (NULL);
	i = 0;
	while (i < size)
	{
		buff[i] = s1[i];
		i++;
	}
	buff[i] = '\0';
	return (buff);
}
