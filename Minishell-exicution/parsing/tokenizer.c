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
    ft_putstr_fd("minishell: syntax error near unexpected token`'\n", 2, 0);
    if (head)
        free_tokens(head, NULL);
    if (val)
        free(val);
}

static int is_quote(char c)
{
    return (c == '\'' || c == '"');
}

t_token *handle_word_with_quotes(char *str, int *i, t_token **head)
{
    int start = *i;
    int in_quotes = 0;
    char quote_type = 0;
    char *val = NULL;
    
    while (str[*i])
    {
        if (is_quote(str[*i]))
        {
            if (!in_quotes)
            {
                in_quotes = 1;
                quote_type = str[*i];
            }
            else if (str[*i] == quote_type)
            {
                in_quotes = 0;
                quote_type = 0;
            }
            (*i)++;
            continue ;
        }
        if (!in_quotes && (ft_isspace(str[*i]) || is_operator(str[*i])))
            break;
            
        (*i)++;
    }

    if (in_quotes)
    {
        print_error(*head, NULL);
        return (NULL);
    }
    val = ft_strndup(&str[start], *i - start);
    if (!val)
        return (NULL);
        
    t_token *new = new_token(val, TOKEN_WORD);
    if (!new)
        return (free(val), NULL);
        
    add_token(head, new);
    return (*head);
}

t_token *handle_operator(char *str, int *i, t_token **head)
{
    char *val = NULL;
    t_token_type type;
    
    if (str[*i] == '\\' || str[*i] == ';')
        return (NULL);
    else if ((str[*i] == '>' || str[*i] == '<') && str[*i] == str[*i+1])
    {
        val = ft_strndup(&str[*i], 2);
        *i += 2;
    }
    else
        val = ft_strndup(&str[(*i)++], 1);
    type = get_token_type(val);

    t_token *new = new_token(val, type);
    if (!new)
        return (free(val), NULL);
        
    add_token(head, new);
    return (*head);
}

t_token *string_tokens(char *str)
{
    t_token *head = NULL;
    int i = 0;

    if (str[i] == '|')
        return (NULL);
        
    while (str[i])
    {
        while (ft_isspace(str[i]))
           i++;       
        if (!str[i])
            break ;
        if (is_operator(str[i]))
        {
            if (!handle_operator(str, &i, &head))
                return (NULL);
        }
        else
        {
            if (!handle_word_with_quotes(str, &i, &head))
                return (NULL);
        }
    }
    
    return (head);
}