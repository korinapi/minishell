/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 18:05:57 by marvinleibe       #+#    #+#             */
/*   Updated: 2024/03/25 23:15:48 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_fprintf(int fd, const char *format, ...)
{
	int len;
	va_list args;

	len = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (*format == 's')
				len += ft_putstr_fd(va_arg(args, char *), fd);
			else if (*format == 'd')
				len += ft_putnbr_fd(va_arg(args, int), fd);
			else if (*format == 'c')
				len += ft_putchar_fd(va_arg(args, int), fd);
		}
		else
			len += ft_putchar_fd(*format, fd);
		format++;
	}
	va_end(args);
	return (len);
}