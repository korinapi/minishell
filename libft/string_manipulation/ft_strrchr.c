/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 14:50:14 by mleibeng          #+#    #+#             */
/*   Updated: 2023/10/31 23:17:58 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int				i;
	unsigned char	ch;

	i = ft_strlen(str);
	ch = (unsigned char)c;
	while (i >= 0)
	{
		if (str[i] == ch)
			return ((char *) &str[i]);
		i--;
	}
	return (NULL);
}

// #include <stdio.h>

// int main(void)
// {
//     char s[ ] = "abcbc";
//     printf( "this should return c so it is %s\n", ft_strrchr(s, 'c'));
//     return 0;
// }