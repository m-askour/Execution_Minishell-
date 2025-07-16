/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 18:14:53 by maskour           #+#    #+#             */
/*   Updated: 2025/04/24 10:31:28 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static size_t	countstr(char const *s, char c)
{
	int		j;
	size_t	count;

	j = 0;
	count = 0;
	while (s[j] != '\0')
	{
		while (s[j] == c && s[j] != '\0')
			j++;
		if (s[j] != c && s[j] != '\0')
			count++;
		while (s[j] != c && s[j] != '\0')
			j++;
	}
	return (count);
}

static char	**ft_free(char **stor, int j)
{
	while (j > 0)
	{
		free(stor[j - 1]);
		j--;
	}
	free(stor);
	return (NULL);
}

static void	strncopy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (src[i] != '\0' && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

static size_t	ft_start(char const *s, char c, size_t *i)
{
	while (s[*i] == c && s[*i] != '\0')
		(*i)++;
	return (*i);
}

char	**ft_split_up(char const *s, char c)
{
	size_t		i;
	size_t		start;
	size_t		nm_str;
	char		**stor;

	i = 0;
	nm_str = 0;
	if (!s && !c)
		return (NULL);
	stor = malloc(sizeof(char *) * (countstr(s, c) + 1));
	if (!stor)
		return (NULL);
	while (s[i] && nm_str < countstr(s, c))
	{
		start = ft_start(s, c, &i);
		while (s[i] != c && s[i] != '\0')
			i++;
		stor[nm_str] = malloc((i - start + 1) * sizeof(char));
		if (!stor[nm_str])
			return (ft_free(stor, nm_str));
		strncopy(stor[nm_str++], &s[start], i - start);
	}
	stor[nm_str] = NULL;
	return (stor);
}
