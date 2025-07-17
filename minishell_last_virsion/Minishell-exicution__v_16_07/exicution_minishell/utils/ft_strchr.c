/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:58:25 by maskour           #+#    #+#             */
/*   Updated: 2025/04/24 10:31:32 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_strchr(const char *src, int c)
{
	size_t	i;

	i = 0;
	while (src[i] != '\0')
	{
		if (src[i] == (char)c)
			return ((char *)(i + src));
		i++;
	}
	if ((char)c == '\0')
		return ((char *)(i + src));
	return (NULL);
}
