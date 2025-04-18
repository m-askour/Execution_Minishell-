/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_Prints_hexadecimal.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:55:21 by maskour           #+#    #+#             */
/*   Updated: 2024/11/25 10:21:54 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_prints_hexadecimal(unsigned int nb, char c)
{
	int				count;
	char			*base;

	count = 0;
	if (c == 'x')
		base = "0123456789abcdef";
	else if (c == 'X')
		base = "0123456789ABCDEF";
	if (nb >= 16)
	{
		count += ft_prints_hexadecimal(nb / 16, c);
		count += ft_prints_hexadecimal(nb % 16, c);
	}
	else
		count += write (1, &base[nb], 1);
	return (count);
}
