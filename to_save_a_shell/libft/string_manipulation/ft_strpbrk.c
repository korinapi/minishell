/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strpbrk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 22:45:08 by mleibeng          #+#    #+#             */
/*   Updated: 2024/03/21 22:47:45 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strpbrk(const char *str1, const char *str2)
{
	const char	*p;
	const char	*spanp;
	char		c;
	char		sc;

	p = str1;
	while ((c = *p++) != '\0')
	{
		spanp = str2;
		while ((sc = *spanp++) != '\0')
		{
			if (c == sc)
				return (char *)(p - 1);
		}
	}
	return (NULL);
}
