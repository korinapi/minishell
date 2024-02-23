/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 16:27:07 by cpuiu             #+#    #+#             */
/*   Updated: 2023/10/06 17:48:43 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	*ch;

	str = (unsigned char *)s;
	ch = NULL;
	while (n--)
	{
		if (*str != (unsigned char)c)
			str++;
		else
		{
			ch = str;
			break ;
		}
	}
	return (ch);
}
