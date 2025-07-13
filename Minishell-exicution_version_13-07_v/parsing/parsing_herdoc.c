/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari-alaa <ahari-alaa@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-07-11 23:03:29 by ahari-alaa       #+#    #+#             */
/*   Updated: 2025-07-11 23:03:29 by ahari-alaa      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Helper function to safely concatenate strings
static char	*safe_strcat_heredoc(char *dest, char *src)
{
	char	*new_str;
	size_t	dest_len;
	size_t	src_len;

	if (!dest || !src)
		return (dest);
	dest_len = ft_strlen(dest);
	src_len = ft_strlen(src);
	new_str = malloc(dest_len + src_len + 1);
	if (!new_str)
		return (NULL);
	ft_strcpy(new_str, dest);
	ft_strcat(new_str, src);
	free(dest);
	return (new_str);
}

// Process quoted section
static char	*process_quoted_heredoc(char *val, int *i)
{
	char	quote;
	int		start;

	quote = val[(*i)++];
	start = *i;
	while (val[*i] && val[*i] != quote)
		(*i)++;
	if (val[*i] == quote)
		(*i)++;
	return (ft_strndup(val + start, (*i) - start - 1));
}

// Process unquoted section
static char	*process_unquoted_heredoc(char *val, int *i)
{
	int		start;
	char	*tmp;

	start = *i;
	while (val[*i] && val[*i] != '\'' && val[*i] != '\"')
		(*i)++;
	tmp = ft_strndup(val + start, (*i) - start);
	if (!tmp)
		return (NULL);
	if (ft_strcmp(tmp, "$") == 0)
	{
		free(tmp);
		tmp = ft_strdup("\1");
	}
	return (tmp);
}

// Handle quoted section processing
static char	*handle_quoted_heredoc(char *result, char *val, int *i)
{
	char	*tmp;
	char	*new_result;
	tmp = process_quoted_heredoc(val, i);
	if (!tmp)
		return (free(result), NULL);
	new_result = safe_strcat_heredoc(result, tmp);
	free(tmp);
	return (new_result);
}

// Handle unquoted section processing
static char	*handle_unquoted_heredoc(char *result, char *val, int *i)
{
	char	*tmp;
	char	*new_result;

	tmp = process_unquoted_heredoc(val, i);
	if (!tmp)
		return (free(result), NULL);
	new_result = safe_strcat_heredoc(result, tmp);
	free(tmp);
	return (new_result);
}

// Main heredoc parsing function
char    *herdoc_parsing(char *val)
{
    char    *result;
    char    *tmp;
    int        i;

    i = 0;
    if (!val)
        return (NULL);
    result = ft_strdup("");
    if (!result)
        return (NULL);
    while (val[i])
    {
        if (val[i] == '\'' || val[i] == '\"')
            tmp = handle_quoted_heredoc(result, val, &i);
        else
            tmp = handle_unquoted_heredoc(result, val, &i);
        if (!tmp)
        {
            free(result);
            return (NULL);
        }
        result = tmp;
    }
    if (ft_strcmp(result, "\1") == 0)
    {
        free(result);
        result = ft_strdup("$");
        if (!result)
            return (NULL);
    }
    tmp = remove_char(result, '\1');
    free(result);
    return tmp;
}
