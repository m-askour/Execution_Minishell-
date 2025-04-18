/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_Prints_decimal.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 11:55:52 by maskour           #+#    #+#             */
/*   Updated: 2024/11/23 12:55:26 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_prints_decimal(int n)
{
	char	digit;
	int		count;

	count = 0;
	if (n == -2147483648)
		return (write(1, "-2147483648", 11));
	else if (n < 0)
	{
		count += write(1, "-", 1);
		n = n * -1;
		count += ft_prints_decimal(n);
	}
	else if (n > 9)
	{
		count += ft_prints_decimal(n / 10);
		count += ft_prints_decimal(n % 10);
	}
	else
	{
		digit = n + '0';
		count += write (1, &digit, 1);
	}
	return (count);
}
