/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 03:04:25 by ahari             #+#    #+#             */
/*   Updated: 2025/06/25 14:58:02 by ahari            ###   ########.fr       */
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

int is_only_spaces(const char *str)
{
    if (!str)
        return 1;

    while (*str)
    {
        if (!ft_isspace((unsigned char)*str))
            return 0;
        str++;
    }
    return 1;
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
            if (*tmp && !is_only_spaces(tmp))
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

static int is_single_quoted(char *original_val, char *substring)
{
    char *pos = ft_strstr(original_val, substring);
    int quote_count = 0;
    int i = 0;
    
    if (!pos)
        return 0;
    
    while (&original_val[i] < pos)
    {
        if (original_val[i] == '\'' && (i == 0 || original_val[i - 1] != '\\'))
            quote_count++;
        i++;
    }
    return (quote_count % 2 == 1);
}

static int process_token(t_token *current, t_token *head, t_shell *shell_ctx, char **env_table) 
{
    char **new_val = NULL;
    char *val_cmd = NULL;
    int i;
    
    if (current->type != TOKEN_WORD)
        return (1);
    
    new_val = process_quoted_value(current->value, head, shell_ctx);
    if (!new_val)
        return (0);
    
    i = 0;
    while (new_val[i])
    {
        if (!is_single_quoted(current->value, new_val[i]))
        {
            new_val[i] = found_env(new_val[i], env_table, shell_ctx);
            if (!new_val[i])
            {
                free_array(new_val);
                free_tokens(head, NULL);
                return (0);
            }
            
            if (ft_strchr(new_val[i], ' ') != NULL)
            {
                char **tmp = ft_split(new_val[i]);
                if (!tmp)
                {
                    free_array(new_val);
                    free_tokens(head, NULL);
                    return (0);
                }
                
                // Calculate how many tokens we got from splitting
                int token_count = 0;
                while (tmp[token_count])
                    token_count++;
                
                // We need to expand the new_val array to accommodate the split tokens
                // This is a complex operation that requires reallocating the array
                // For now, we'll handle the first token and note that proper expansion is needed
                
                free(new_val[i]);
                new_val[i] = ft_strdup(tmp[0]);
                if (!new_val[i])
                {
                    free_array(tmp);
                    free_array(new_val);
                    free_tokens(head, NULL);
                    return (0);
                }
                
                // TODO: Properly handle multiple tokens from splitting
                // This requires expanding the new_val array and adjusting indices
                
                free_array(tmp);
            }
        }
        
        if (i == 0)
        {
            val_cmd = ft_strdup(new_val[i]);
        }
        else
        {
            char *tmp = ft_strjoin(val_cmd, " ");
            if (!tmp)
            {
                free(val_cmd);
                free_array(new_val);
                free_tokens(head, NULL);
                return (0);
            }
            free(val_cmd);
            val_cmd = tmp;
            
            tmp = ft_strjoin(val_cmd, new_val[i]);
            if (!tmp)
            {
                free(val_cmd);
                free_array(new_val);
                free_tokens(head, NULL);
                return (0);
            }
            free(val_cmd);
            val_cmd = tmp;
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
        return (NULL);
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
