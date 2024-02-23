/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 14:49:29 by mleibeng          #+#    #+#             */
/*   Updated: 2023/10/31 23:15:51 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memchr(const void *b, int c, size_t size)
{
	size_t			i;
	unsigned char	cc;

	i = 0;
	cc = (unsigned char )c;
	while (i < size)
	{
		if (((unsigned char *)b)[i] == cc)
			return ((void *)(b + i));
		i++;
	}
	return ((NULL));
}
