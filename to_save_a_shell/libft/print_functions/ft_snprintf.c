/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvinleibenguth <marvinleibenguth@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 23:22:24 by marvinleibe       #+#    #+#             */
/*   Updated: 2024/04/03 22:16:11 by marvinleibe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	custom_ft_itoa(int n, char *buf)
{
	int	i;
	int	is_negative;
	int	temp;

	i = 0;
	is_negative = 0;
	if (n == 0)
	{
		buf[i++] = '0';
		buf[i] = '\0';
		return ;
	}
	if (n < 0)
	{
		is_negative = 1;
		n = -n;
	}
	temp = n;
	while (temp)
	{
		i++;
		temp /= 10;
	}
	buf[i] = '\0';
	i--;
	if (is_negative)
	{
		buf[0] = '-';
	}
	while (n)
	{
		buf[is_negative + i--] = (n % 10) + '0';
		n /= 10;
	}
}

int	ft_snprintf(char *str, int size, const char *format, ...)
{
	va_list args;
	int len;
	const char *fmt;
	char *ptr;
	char *str_val;
	int int_val;
	int num_len;
	char char_val;
	char buf[20];

	va_start(args, format);
	len = 0;
	ptr = str;
	fmt = format;

	while (*fmt && (ptr - str) < size - 1)
	{
		if (*fmt == '%')
		{
			fmt++;
			if (*fmt == 's')
			{
				str_val = va_arg(args, char *);
				num_len = ft_strlen(str_val);
				if (num_len < size - (ptr - str))
				{
					ft_strcpy(ptr, str_val);
					ptr += num_len;
				}
				else
				{
					ft_strncpy(ptr, str_val, size - (ptr - str) - 1);
					ptr += size - (ptr - str) - 1;
				}
				len += num_len;
			}
			else if (*fmt == 'd')
			{
				int_val = va_arg(args, int);
				custom_ft_itoa(int_val, buf);
				num_len = ft_strlen(buf);
				if (num_len < size - (ptr - str))
				{
					ft_strcpy(ptr, buf);
					ptr += num_len;
				}
				else
				{
					ft_strncpy(ptr, buf, size - (ptr - str) - 1);
					ptr += size - (ptr - str) - 1;
				}
				len += num_len;
			}
			else if (*fmt == 'c')
			{
				char_val = (char)va_arg(args, int);
				if ((ptr - str) < size - 1)
				{
					*ptr++ = char_val;
					len++;
				}
				else
				{
					len = -1;
					break ;
				}
			}
			fmt++;
		}
		else
		{
			if ((ptr - str) < size - 1)
			{
				*ptr++ = *fmt++;
				len++;
			}
			else
			{
				len = -1;
				break ;
			}
		}
	}

	if (size > 0)
	{
		*ptr = '\0';
	}

	va_end(args);
	return (len >= 0 ? len : len + size);
}