/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:04:38 by ahari             #+#    #+#             */
/*   Updated: 2025/04/24 16:12:36 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void print_error(t_token *head, char *val)
{
    ft_putstr_fd("minishell: syntax error near unexpected token`'\n",2,0);
    if(head)
        free_tokens(head, NULL);
    if(val)
        free(val);
}

t_token *handle_operator_or_word(char *str, int *i, int start, t_token **head)
{
    char *val = NULL;
    t_token_type type;

    if (is_operator(str[*i]))
    {
        if (str[*i] == '\\' || str[*i] == ';')
            return (free(val), NULL);
        else if ((str[*i] == '>' || str[*i] == '<') && str[*i] == str[*i+1])
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
        val = ft_strndup(&str[start], *i - start);
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

    if(str[i] == '|')
        return (NULL);
    while (str[i])
    {
        while (ft_isspace(str[i]))
            i++;
        if (!str[i])
            break;
        start = i;
        if (!handle_operator_or_word(str, &i, start, &head))
                return (NULL);
    }
    return (head);
}