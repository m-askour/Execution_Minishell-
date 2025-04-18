/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_Prints_unsigned_decimal.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:49:14 by maskour           #+#    #+#             */
/*   Updated: 2024/11/23 12:55:41 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_prints_unsigned_decimal(unsigned int nb)
{
	char	digit;
	int		count;

	count = 0;
	if (nb > 9)
	{
		count += ft_prints_unsigned_decimal(nb / 10);
		count += ft_prints_unsigned_decimal(nb % 10);
	}
	else
	{
		digit = nb + '0';
		count += write(1, &digit, 1);
	}
	return (count);
}
