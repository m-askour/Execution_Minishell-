/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 22:21:04 by maskour           #+#    #+#             */
/*   Updated: 2025/06/20 21:51:45 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../minishell.h"

void	print_error(t_token *head, char *val, t_shell *shell_ctx)
{
	ft_putstr_fd("minishell: syntax error near unexpected token`'\n", 2, 0);
	shell_ctx->exit_status = 2;
	if (head)
		free_tokens(head, NULL);
	if (val)
		free(val);
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

static int	ft_intlen(int n)
{
	int	len;

	if (n == 0)
		return (1);
	len = 0;
	if (n < 0)
		len++;
	while (n != 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*str;
	long	nb = (long)n;

	len = ft_intlen(n);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (nb == 0)
	{
		str[0] = '0';
		return (str);
	}
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	while (nb > 0)
	{
		str[--len] = (nb % 10) + '0';
		nb /= 10;
	}
	return (str);
}
char *ft_strjoin(char const *s1, char const *s2)
{
    char *result;
    int i, j;

    if (!s1 || !s2)
        return (NULL);
    
    result = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
    if (!result)
        return (NULL);
    
    i = 0;
    while (s1[i])
    {
        result[i] = s1[i];
        i++;
    }
    
    j = 0;
    while (s2[j])
    {
        result[i + j] = s2[j];
        j++;
    }
    result[i + j] = '\0';
    return (result);
}
char	*ft_strcpy(char *dest, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
