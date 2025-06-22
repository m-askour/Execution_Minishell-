/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 03:04:25 by ahari             #+#    #+#             */
/*   Updated: 2025/06/20 23:03:57 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void free_array(char **array)
{
    int i;

    if (!array)
        return;
    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}
char **process_quoted_value(char *val, t_token *head, t_shell *shell_ctx)
{
    char    **strings = NULL;
    int     i = 0, start, count = 0;
    char    quote;
    char    *tmp;

    if (!val)
        return (print_error(head, NULL, shell_ctx), NULL);
    strings = malloc(sizeof(char *) * (ft_strlen(val) + 1));
    if (!strings)
        return (print_error(head, NULL, shell_ctx), NULL);
    while (val[i])
    {
        if (val[i] == '\'' || val[i] == '\"')
        {
            quote = val[i];
            i++;
            start = i;
            while (val[i] && val[i] != quote)
                i++;
            tmp = ft_substr(val, start, i - start);
            if (!tmp)
            {
                free_array(strings);
                return (print_error(head, NULL, shell_ctx), NULL);
            }
            if (*tmp)
                strings[count++] = tmp;
            else
                free(tmp);
            if (val[i] == quote)
                i++;
        }
        else
        {
            start = i;
            while (val[i] && val[i] != '\'' && val[i] != '\"')
                i++;
            tmp = ft_substr(val, start, i - start);
            if (!tmp)
            {
                free_array(strings);
                return (print_error(head, NULL, shell_ctx), NULL);
            }
            if (*tmp)
                strings[count++] = tmp;
            else
                free(tmp);
        }
    }
    strings[count] = NULL;
    return strings;
}

static int process_token(t_token *current,t_token *head, t_shell *shell_ctx, char **env_table)
{
    char **new_val = NULL;
    char *val_cmd = NULL;
    int i;
    
    if (current->type != TOKEN_WORD)
        return (1);
    new_val = process_quoted_value(current->value, head, shell_ctx);
    if (!new_val)
        return (free_tokens(head, NULL), 0);
    i = 0;
    while(new_val[i])
    {
        new_val[i] = found_env(new_val[i], env_table, shell_ctx);
        if (!new_val[i])
        {
            free_array(new_val);
            free_tokens(head, NULL);
            return (0);
        }
        if (i == 0)
            val_cmd = ft_strdup(new_val[i]);
        else
        {
            char *tmp = ft_strjoin(val_cmd, new_val[i]);
            free(val_cmd);
            val_cmd = tmp;
            if (!val_cmd)
            {
                free_array(new_val);
                free_tokens(head, NULL);
                return (0);
            }
        }
        i++;
    }
    free_array(new_val);
    free(current->value);
    if (val_cmd != NULL)
        current->value = val_cmd;
    else
        current->value = ft_strdup("");
    if (!current->value)
    {
        free_tokens(head, NULL);
        return (0);
    }
    return (1);
}

t_token *check_quoted(char *str, t_shell *shell_ctx , char **env_table)
{
    t_token *head;
    t_token *current;

    head = string_tokens(str, shell_ctx);
    if (!head)
        return (shell_ctx->exit_status = 2, NULL);
    current = head;
    while (current)
    {
        if (!process_token(current, head, shell_ctx , env_table))
        {
            free_tokens(head, NULL);
            free_char_array(env_table);
            return NULL;
        }
        current = current->next;
    }
    return head;
}
