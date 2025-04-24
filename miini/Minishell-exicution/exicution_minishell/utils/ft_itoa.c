/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:16:31 by maskour           #+#    #+#             */
/*   Updated: 2025/04/20 17:27:31 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	digit_int(long nb)
{
	int	count;

	count = 0;
	if (nb < 0)
	{
		count++;
		nb = nb * -1;
	}
	if (nb == 0)
		count++;
	while (nb != 0)
	{
		nb = nb / 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*loc_str;
	int		i;
	long	nb;

	nb = n;
	loc_str = malloc(sizeof(char) * (digit_int(n) + 1));
	if (!loc_str)
		return (NULL);
	if (nb < 0)
	{
		loc_str[0] = '-';
		nb = nb * -1;
	}
	i = digit_int(n) - 1;
	if (nb == 0)
		loc_str[0] = '0';
	while (nb > 0)
	{
		loc_str[i] = (nb % 10) + '0';
		nb = nb / 10;
		i--;
	}
	loc_str[digit_int(n)] = '\0';
	return (loc_str);
}
