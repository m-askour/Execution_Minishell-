/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:23:16 by maskour           #+#    #+#             */
/*   Updated: 2025/04/20 18:07:35 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*c;
	size_t	len1;
	size_t	len2;
	size_t	len;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len = len1 + len2;
	c = (char *)malloc(sizeof(char) * (len + 1));
	if (!c)
		return (NULL);
	ft_strlcpy(c, s1, len1 + 1);
	ft_strlcat(c, s2, len + 1);
	return (c);
}
