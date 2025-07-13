/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:17:31 by ahari             #+#    #+#             */
/*   Updated: 2025/07/13 22:19:31 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char *realloc_result(char *result, size_t *capacity, size_t new_len)
{
    char    *new_result;
    size_t  old_capacity;

    if (new_len <= *capacity)
        return (result);
    old_capacity = *capacity;
    *capacity = new_len * 2;
    new_result = ft_realloc(result, old_capacity, *capacity);
    if (!new_result)
    {
        free(result);
        return (NULL);
    }
    return (new_result);
}

// Helper function to append string to result with reallocation
static char *append_to_result(char *result, char *tmp, size_t *capacity)
{
    size_t  result_len;
    char    *new_result;
    
    if (!tmp)
        return (result);
    result_len = ft_strlen(result) + ft_strlen(tmp) + 1;
    new_result = realloc_result(result, capacity, result_len);
    if (!new_result)
        return (free(tmp), free(result), NULL);
    ft_strcat(new_result, tmp);
    free(tmp);
    return (new_result);
}

// Process quoted section (single or double quotes)
static char *process_quoted_section(char *val, int *i, t_env_list *env)
{
    char quote;
    int start;
    char *tmp;
    char *expanded;

    quote = val[(*i)++];
    start = *i;
    while (val[*i] && val[*i] != quote)
        (*i)++;
    tmp = ft_substr(val, start, *i - start);
    if (!tmp)
        return (NULL);
    if (quote == '\"')
    {
        expanded = found_env(tmp, env->env_table, env->exit_status);
        free(tmp);
        tmp = expanded;
    }
    if (val[*i] == quote)
        (*i)++;
    return (tmp);
}

// Process unquoted section with environment variable expansion
static char *process_unquoted_section(char *val, int *i, t_env_list *env)
{
    int start;
    char *tmp;
    char *expanded;
    
    start = *i;
    while (val[*i] && val[*i] != '\'' && val[*i] != '\"')
        (*i)++;
    tmp = ft_substr(val, start, *i - start);
    if (!tmp)
        return (NULL);
    expanded = found_env(tmp, env->env_table, env->exit_status);
    free(tmp);
    tmp = expanded;
    if (ft_strcmp(tmp, "$") == 0)
    {
        free(tmp);
        tmp = ft_strdup("\1");
    }
    return (tmp);
}

// Initialize result buffer
static char *init_result_buffer(char *val, t_token *head, size_t *capacity)
{
    char *result;
    
    if (!val)
        return (print_error(head, NULL, NULL), NULL);
    *capacity = ft_strlen(val) + 1;
    result = malloc(*capacity);
    if (!result)
        return (print_error(head, NULL, NULL), NULL);
    result[0] = '\0';
    return (result);
}

// Main function - now less than 25 lines
char *process_quoted_value(char *val, t_token *head, t_env_list *env)
{
    char *result;
    char *tmp;
    int i;
    size_t result_capacity;
    char *temp;

    temp = NULL;
    i = 0;
    if (ft_strchr(val, '=') != NULL)
        val = remove_dollar_before_quotes(val, &temp);
    if(!val)
        return NULL;
    result = init_result_buffer(val, head, &result_capacity);
    if (!result)
        return (NULL);
    while (val[i])
    {
        if (val[i] == '\'' || val[i] == '\"')
            tmp = process_quoted_section(val, &i, env);
        else
            tmp = process_unquoted_section(val, &i, env);
        if (!tmp)
            return (free(result), free(temp), NULL);
        result = append_to_result(result, tmp, &result_capacity);
        if (!result)
            return (free(val),free(temp), NULL);
    }
    if (ft_strcmp(result, "\1") == 0)
    {
        free(result);
        result = ft_strdup("$");
    }
    return (free(temp), result);
}
