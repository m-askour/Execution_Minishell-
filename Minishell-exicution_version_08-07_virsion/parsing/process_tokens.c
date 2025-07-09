/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:17:31 by ahari             #+#    #+#             */
/*   Updated: 2025/07/08 18:19:32 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *ft_strjoin3(char const *s1, char const *s2, char const *s3)
{
    char    *result;
    size_t  len1;
    size_t  len2;
    size_t  len3;
    size_t  total_len;

    if (!s1 || !s2 || !s3)
        return (NULL);
    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);
    len3 = ft_strlen(s3);
    total_len = len1 + len2 + len3 + 1;
    result = (char *)malloc(total_len * sizeof(char));
    if (!result)
        return (NULL);
    ft_strlcpy(result, s1, len1 + 1);
    ft_strlcpy(result + len1, s2, len2 + 1);
    ft_strlcpy(result + len1 + len2, s3, len3 + 1);
    return (result);
}
void export_one_case(char *value, t_token *head)
{
    char *equal_sign;
    char *var_part;
    char *val_part;
    char *new_value;
    char *temp;
    int needs_quotes = 0;
    int has_empty_quotes;

    if (!value || !head)
        return;
    equal_sign = ft_strchr(value, '=');
    if (!equal_sign)
        return;
    var_part = ft_strndup(value, equal_sign - value);
    val_part = ft_strdup(equal_sign + 1);
    if (!var_part || !val_part)
    {
        free(var_part);
        free(val_part);
        return ;
    }
    has_empty_quotes = (var_part[0] == '\'' && var_part[1] == '\'');
    if (ft_strchr(val_part, '$') && 
        !is_quoted(val_part, val_part) &&
        !has_empty_quotes)
        needs_quotes = 1;
    if (needs_quotes)
    {
        new_value = ft_strjoin3(var_part, "=\"", val_part);
        if (new_value)
        {
            temp = ft_strjoin(new_value, "\"");
            free(new_value);
            new_value = temp;
        }
    }
    else
        new_value = ft_strjoin3(var_part, "=", val_part);
    free(var_part);
    free(val_part);
    if (new_value)
    {
        free(head->value);
        head->value = new_value;
        head->was_quoted = needs_quotes ? 1 : 0;
    }
}

void remove_quotes_before_equal(t_token *token)
{
    if (!token || !token->value)
        return;
    char *equal_sign = ft_strchr(token->value, '=');
    if (!equal_sign)
        return;
    int prefix_len = equal_sign - token->value;
    char *src = token->value;
    char *clean = malloc(ft_strlen(token->value) + 1);
    if (!clean)
        return;
    int i = 0, j = 0;
    char quote = 0;
    while (i < prefix_len)
    {
        if ((src[i] == '\'' || src[i] == '\"') && quote == 0)
            quote = src[i];
        else if (src[i] == quote)
            quote = 0;
        else
            clean[j++] = src[i];
        i++;
    }
    while (src[i])
        clean[j++] = src[i++];
    clean[j] = '\0';
    free(token->value);
    token->value = clean;
}
