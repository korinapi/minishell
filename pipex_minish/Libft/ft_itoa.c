/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 10:44:15 by cpuiu             #+#    #+#             */
/*   Updated: 2023/10/07 12:13:36 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_nblen(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		count;
	long	nb;

	nb = n;
	count = ft_nblen(nb);
	str = (char *)malloc(sizeof(char) * (count + 1));
	if (!str)
		return (NULL);
	str[count] = '\0';
	if (nb == 0)
		str[0] = '0';
	while (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	while (nb && count--)
	{
		str[count] = (nb % 10) + 48;
		nb /= 10;
	}
	return (str);
}
