/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strspn.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 22:42:34 by mleibeng          #+#    #+#             */
/*   Updated: 2024/03/25 23:17:17 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_strspn(const char *str1, const char *str2)
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
				break ;
		}
		if (sc == '\0')
			return (p - 1 - str1);
	}
	return (p - 1 - str1);
}
