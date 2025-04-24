/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 03:04:47 by ahari             #+#    #+#             */
/*   Updated: 2025/04/24 12:09:17 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

char    *ft_strndup(const char *s, size_t n)
{

    char *dup = malloc(n + 1);
    if (!dup)
        return NULL;

    size_t i = 0;
    while (i < n && s[i] != '\0')
    {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';

    return dup;
}

// char    *ft_strdup(const char *s1)
// {
//     size_t len = 0;
//     char *dup;
//     size_t i;

//     if (!s1)
//         return (NULL);
//     while (s1[len])
//         len++;
//     dup = (char *)malloc((len + 1) * sizeof(char));
//     if (!dup)
//         return (NULL);
//     i = 0;
//     while (s1[i])
//     {
//         dup[i] = s1[i];
//         i++;
//     }
//     dup[i] = '\0';
//     return dup;
// }
void	ft_putchar_fd(char c, int fd)
{
	if (fd >= 0)
		write(fd, &c, 1);
}

void ft_putstr_fd(char *s, int fd, char c)
{
    int i;

    if (!s)
        return;
    i = 0;
    while (s[i])
    {
        if (s[i] == '%' && s[i + 1] == 'c')
        {
            ft_putchar_fd(c, fd);
            i += 2;
        }
        else
        {
            ft_putchar_fd(s[i], fd);
            i++;
        }
    }
}

