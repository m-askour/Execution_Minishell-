/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 03:04:25 by ahari             #+#    #+#             */
/*   Updated: 2025/04/24 16:05:08 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char *process_quoted_value(char *val, t_token *head)
{
    int     len;
    char    *new_val;
    int     i;
    int     j;

    i = 0;
    j = 0;
    if (!val)
        return (print_error(head, NULL), NULL);
    len = ft_strlen(val);
    new_val = malloc(len + 1);
    if (!new_val)
        return (print_error(head, NULL), NULL);
    while (i < len)
    {
        if (val[i] == '\'' || val[i] == '\"')
        {
            char quote = val[i];
            new_val[j++] = val[i++];
            while (i < len && val[i] != quote)
                new_val[j++] = val[i++];
            if (i < len)
                new_val[j++] = val[i++];
            else
                return (print_error(NULL, new_val), NULL);
        } 
        else
            new_val[j++] = val[i++];
    }
    new_val[j] = '\0';
    return new_val;
}


static int process_token(t_token *current, t_token *head)
{
    char *new_val;
    char *val;
    
    if (current->type != TOKEN_WORD)
        return (1); 
    val = current->value;
    new_val = process_quoted_value(val, head);
    if (!new_val)
        return (free_tokens(head, NULL), 0);
    free(current->value);
    current->value = new_val;
    return (1);
}

t_token *check_quoted(char *str)
{
    t_token *head;
    t_token *current;

    head = string_tokens(str);
    current = head;
    if (!head)
        return (NULL);
    while (current)
    {
        if (!process_token(current, head))
            return NULL;
        current = current->next;
    }
    return head;
}
