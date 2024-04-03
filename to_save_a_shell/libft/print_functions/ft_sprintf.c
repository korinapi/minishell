/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 01:33:50 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/03 19:02:07 by mleibeng         ###   ########.fr       */
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

int	ft_sprintf(char *str, const char *format, ...)
{
	va_list		args;
	int			len;
	const char	*fmt;
	char		*ptr;
	char		*str_val;
	int			int_val;
	int			num_len;
	char		char_val;
	char		buf[20];

	va_start(args, format);
	len = 0;
	ptr = str;
	fmt = format;
	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt++;
			if (*fmt == 's')
			{
				str_val = va_arg(args, char *);
				len += strlen(str_val);
				ft_strcpy(ptr, str_val);
				ptr += strlen(str_val);
			}
			else if (*fmt == 'd')
			{
				int_val = va_arg(args, int);
				custom_ft_itoa(int_val, buf);
				num_len = strlen(buf);
				len += num_len;
				ft_strcpy(ptr, buf);
				ptr += num_len;
			}
			else if (*fmt == 'c')
			{
				char_val = (char)va_arg(args, int);
				*ptr++ = char_val;
				len++;
			}
			fmt++;
		}
		else
		{
			*ptr++ = *fmt++;
			len++;
		}
	}
	*ptr = '\0';
	va_end(args);
	return (len);
}
