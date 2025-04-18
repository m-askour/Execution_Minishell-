/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prints_pointer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:23:05 by maskour           #+#    #+#             */
/*   Updated: 2024/11/23 14:34:40 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_prints_pointer(unsigned long nb)
{
	int		count;
	char	*base;

	count = 0;
	base = "0123456789abcdef";
	if (nb >= 16)
	{
		count += ft_prints_pointer(nb / 16);
		count += ft_prints_pointer(nb % 16);
	}
	else
		count += write(1, &base[nb], 1);
	return (count);
}
