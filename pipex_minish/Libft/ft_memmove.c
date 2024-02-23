/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 13:04:26 by cpuiu             #+#    #+#             */
/*   Updated: 2023/10/06 10:42:50 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t len)
{
	char	*cdest;
	char	*csrc;

	cdest = (char *)dest;
	csrc = (char *)src;
	if (!cdest && !csrc)
		return (NULL);
	if (cdest < csrc)
	{
		while (len)
		{
			*cdest++ = *csrc++;
			len--;
		}
	}
	else
	{
		cdest += len;
		csrc += len;
		while (len--)
			*(--cdest) = *(--csrc);
	}
	return (dest);
}
