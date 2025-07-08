/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_pasing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 03:16:06 by ahari             #+#    #+#             */
/*   Updated: 2025/06/22 20:43:41 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	ft_isalpha(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}
size_t	ft_strlen(const char *s)
{
	size_t	i;
	i = 0;
	while (s[i])
	{
		i++;
	}
	return (i);
}

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int is_operator(const char s)
{
    return (s == '|' || s == '<' || s == '>');
}

t_token_type get_token_type(const char *s)
{
    if (ft_strcmp(s, "|") == 0)
        return (TOKEN_PIPE);
    else if (ft_strcmp(s, "<") == 0)
        return (TOKEN_REDIRECT_IN);
    else if (ft_strcmp(s, ">") == 0)
        return (TOKEN_REDIRECT_OUT);
    else if (ft_strcmp(s, "<<") == 0)
        return (TOKEN_HEREDOC);
    else if (ft_strcmp(s, ">>") == 0)
        return (TOKEN_APPEND);
    else
        return (TOKEN_WORD);
}

void add_token(t_token **head, t_token *new)
{
    t_token *tmp;
    if (!*head)
        *head = new;
    else
    {
        tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}
