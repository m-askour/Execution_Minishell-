/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 01:50:41 by ahari             #+#    #+#             */
/*   Updated: 2025/04/20 07:06:47 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void free_split(char **p)
{
	int i;

	i  = 0;
	while (p[i])
	{
		free(p[i]);
		i++;
	}
	free(p);
}

int count_word(char *str)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == 32 || str[i] == '\t'))
			i++;
		if (str[i])
		{
			count++;
			while (str[i] && str[i] != 32 && str[i] != '\t')
			i++;
		}
	}
	return (count);
}

char *malloc_str(char *str)
{
	int i = 0;
	while (str[i] && str[i] != 32 && str[i] != '\t')
		i++;
	char *p = malloc (i + 1);
	if (!p)
		return NULL;
	i = 0;
	while (str[i] && str[i] != 32 && str[i] != '\t')
	{
		p[i] = str[i];
		i++;
	}
	p[i] = '\0';
	return p;
}

char **ft_split(char *str)
{
	int		i;
	int		j;
	char	**p;
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	p = malloc(sizeof(char *) * (count_word(str) + 1));
	if (!p)
		return (NULL);
	while (i < count_word(str))
	{
		while (str[j] && (str[j] == 32 || str[j] == '\t'))
			j++;
		p[i] = malloc_str(str + j);
		if (!p[i])
			return (free_split(p), NULL);
		while (str[j] && str[j] != 32 && str[j] != '\t')
			j++;
		i++;
	}
	p[i] = (NULL);
	return (p);
}
