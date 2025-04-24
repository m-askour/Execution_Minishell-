/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 03:04:25 by ahari             #+#    #+#             */
/*   Updated: 2025/04/22 00:14:42 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


static char *get_quoted_string(const char *str, int *index, char quote_char)
{
    int start = *index + 1;
    int len = 0;

    while (str[start + len] && str[start + len] != quote_char)
        len++;

    if (str[start + len] != quote_char)
    {
        ft_putstr_fd("minishell: syntax error near unexpected token `%c'\n", 2, quote_char);
        return NULL;
    }
    char *val = malloc(len + 1);
    if (!val)
        return NULL;

    ft_strncpy(val, &str[start], len);
    val[len] = '\0';
    *index = start + len + 1;
    return val;
}

t_token *handle_quoted_string(char *str, int *i, t_token **head)
{
    char *val = get_quoted_string(str, i, str[*i]);
    if (!val)
        return NULL;
    
    t_token *new = new_token(val, TOKEN_WORD);
    if (!new)
    {
        free(val);
        return NULL;
    }
    add_token(head, new);
    return *head;
}

t_token *handle_operator_or_word(char *str, int *i, int start, t_token **head)
{
    char *val = NULL;
    t_token_type type;

    if (is_operator(str[*i]))
    {
        if ((str[*i] == '>' || str[*i] == '<') && str[*i] == str[*i+1])
        {
            val = ft_strndup(&str[*i], 2);
            *i += 2;
        }
        else
            val =ft_strndup(&str[(*i)++], 1);
        type = get_token_type(val);
    }
    else
    {
        while (str[*i] && !ft_isspace(str[*i]) && !is_operator(str[*i]))
            (*i)++;
        val = strndup(&str[start], *i - start);
        type = TOKEN_WORD;
    }
    if (val)
    {
        t_token *new = new_token(val, type);
        if (!new)
            return (free(val), NULL);
        add_token(head, new);
    }
    return (*head);
}


t_token *string_tokens(char *str)
{
    t_token *head = NULL;
    int i = 0;
    int start;
    
    while (str[i])
    {
        while (ft_isspace(str[i]))
            i++;
        if (!str[i])
            break;
        start = i;
        if (str[i] == '\'' || str[i] == '\"')
        {
            if (!handle_quoted_string(str, &i, &head))
                return (free_tokens(head, NULL), NULL);
        }
        else
        {
            if (!handle_operator_or_word(str, &i, start, &head))
                return (free_tokens(head, NULL), NULL);
        }
    }
    
    return head;
}
