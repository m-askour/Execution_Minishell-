/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 09:58:51 by maskour           #+#    #+#             */
/*   Updated: 2024/11/27 10:19:58 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_check(char format, va_list args)
{
	int		cont;
	void	*ptr;

	cont = 0;
	if (format == 'i' || format == 'd')
		cont += ft_prints_decimal(va_arg(args, int));
	else if (format == 'c')
		cont += ft_prints_single_character(va_arg(args, int));
	else if (format == 's')
		cont += ft_prints_string(va_arg(args, const char *));
	else if (format == 'x' || format == 'X')
		cont += ft_prints_hexadecimal(va_arg(args, unsigned int), format);
	else if (format == 'u')
		cont += ft_prints_unsigned_decimal(va_arg(args, unsigned int));
	else if (format == '%')
		cont += write(1, "%", 1);
	else if (format == 'p')
	{
		ptr = va_arg(args, void *);
		cont += write(1, "0x", 2);
		cont += ft_prints_pointer((unsigned long)ptr);
	}
	else
		cont += write(1, &format, 1);
	return (cont);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		i;
	int		cont;

	va_start (args, format);
	i = 0;
	cont = 0;
	if (write(1, "", 0) == -1)
		return (-1);
	while (format[i] != '\0')
	{
		if (format[i] == '%')
		{
			if (format[i + 1] != '\0')
			{
				i++;
				cont += ft_check(format[i], args);
			}
		}
		else
			cont += write(1, &format[i], 1);
		i++;
	}
	va_end (args);
	return (cont);
}
