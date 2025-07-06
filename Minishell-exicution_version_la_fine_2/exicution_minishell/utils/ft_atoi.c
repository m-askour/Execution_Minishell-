/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:55:44 by maskour           #+#    #+#             */
/*   Updated: 2025/05/06 10:13:39 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_swap_sign(const char *str, int *i)
{
	int	sign;

	sign = 1;
	while ((str[*i] >= 9 && str[*i] <= 13) || str[*i] == 32)
		(*i)++;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			sign = sign * -1;
		(*i)++;
	}
	return (sign);
}

int	ft_atoi(const char *str)
{
	int			i;
	long long	nb;
	int			sign;
	long long	result;

	i = 0;
	nb = 0;
	result = 0;
	sign = ft_swap_sign(str, &i);
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = str[i] + (nb * 10) - '0';
		if (nb / 10 < result)
		{
			if (sign == 1)
				return (-1);
			else
				return (0);
		}
		result = nb;
		i++;
	}
	return (result * sign);
}
