/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:04:38 by ahari             #+#    #+#             */
/*   Updated: 2025/06/20 22:50:05 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token *handle_word_with_quotes(char *str, int *i, t_token **head, t_shell *shell_ctx)
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
        print_error(*head, NULL, shell_ctx);
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
    char            *val;
    t_token_type    type;
    t_token         *new;

    val = NULL;
    if (str[*i] == '\\' || str[*i] == ';')
        return (NULL);
    else if ((str[*i] == '>' || str[*i] == '<') && str[*i] == str[*i+1])
    {
        val = ft_strndup(&str[*i], 2);
        *i += 2;
    }
    else
        val = ft_strndup(&str[(*i)++], 1);
    if (!val)
        return (NULL);
    type = get_token_type(val);
    new = new_token(val, type);
    if (!new)
        return (free(val), NULL);
    add_token(head, new);
    return (*head);
}
void ft_putstr(char *s)
{
    if (s)
        write(2, s, ft_strlen(s));
}
t_token *string_tokens(char *str, t_shell *shell_ctx)
{
    t_token *head;
    head = NULL;
    int i = 0;
    char quote = 0;
    while (str[i])
    {
        if ((str[i] == '\'' || str[i] == '"') && (quote == 0))
            quote = str[i];
        else if (str[i] == quote)
            quote = 0;
        if (!quote)
        {
            if (str[0] == '|'
                || (str[i - 1] == '|' && str[i] == '|'))
                return (write( 2 , "syntax error near unexpected token `|'\n", 40),free_tokens(head, NULL), NULL);
            else if (str[i - 1] == ';' && str[i] == ';')
                return (write(2, "syntax error near unexpected token `;;'\n",41),free_tokens(head, NULL), NULL);
            else if (str[i] == '\\')
                return (write(2, "syntax error near unexpected token `\\'\n",40),free_tokens(head, NULL), NULL);
            else if (str[i] == ';')
                return (write(2, "syntax error near unexpected token `;'\n",40),free_tokens(head, NULL), NULL);
            else if (str[i] == '!')
                return (write(2, "syntax error near unexpected token `!'\n",40),free_tokens(head, NULL), NULL);
            else if (str[i] == '&')
                return (write(2, "syntax error near unexpected token `&'\n",40),free_tokens(head, NULL), NULL);
            else if (str[i - 1] == ':' && str[i] == ':')
                return (write(2, "syntax error near unexpected token `::'\n",41),free_tokens(head, NULL), NULL);
            else if (str[i] == '(' || str[i] == ')')
                return (write(2, "syntax error near unexpected token `()'\n",41),free_tokens(head, NULL), NULL);
        }
        i++;
    }
    i = 0;
    while (str[i])
    {
        while (ft_isspace(str[i]))
           i++;       
        if (!str[i])
            break ;
        if (is_operator(str[i]))
        {
            if (!handle_operator(str, &i, &head))
                return (free_tokens(head, NULL), NULL);
        }
        else
        {
            if (!handle_word_with_quotes(str, &i, &head, shell_ctx))
                return (free_tokens(head, NULL), NULL);
        }
    } 
    return (head);
}
