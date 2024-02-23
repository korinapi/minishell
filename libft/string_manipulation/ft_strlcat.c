/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 14:50:00 by mleibeng          #+#    #+#             */
/*   Updated: 2023/10/31 23:17:38 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	d_len;
	size_t	s_len;
	size_t	i;
	size_t	s_i;

	d_len = ft_strlen(dest);
	s_len = ft_strlen(src);
	i = d_len;
	s_i = 0;
	if (size == 0)
		return (s_len);
	while (i < size - 1 && src[s_i] != '\0')
	{
		dest[i] = src[s_i];
		i++;
		s_i++;
	}
	dest[i] = '\0';
	if (i >= size)
		return (s_len + size);
	else
		return (s_len + d_len);
}

// #include <stdio.h>
// #include <string.h>

// int	main(void)
// {
// 	char d[100] = "Hello, ";
// 	const char s[] = "World";
// 	char e[100] = "Hello, ";
// 	const char f[] = "World";
// 	int result1 = ft_strlcat(d,s,100);
// 	int result2 = strlcat(e,f,100);
// 	printf("%d %d", result1, result2);
// }

// #include <stdio.h>
// #include <string.h>

// int main()
// {
//     char first[] = "This is ";
//     char last[] = "a potentially long string";
// 	char first1[] = "This is ";
//     int r;
// 	int d;
//     int size = 64;
//     char buffer[size];
// 	char buffer1[size];

//     strcpy(buffer,first);
// 	strcpy(buffer1,first1);
// 	d = ft_strlcat(buffer1,last,size);
//     r = strlcat(buffer,last,size);
//     puts(buffer);
//     printf("Value returned: %d\n",d);
//     if( d > size )
//         puts("String truncated");
//     else
//         puts("String was fully copied");
//      printf("Value returned: %d\n",r);
//     if( r > size )
//         puts("String truncated");
//     else
//         puts("String was fully copied");

//     return(0);
// }