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

char	*herdoc_parsing_sections(char *val, char **result_ptr)
{
	char	*tmp;
	int		i;

	i = 0;
	while (val[i])
	{
		if (val[i] == '\'' || val[i] == '\"')
			tmp = handle_quoted_heredoc(*result_ptr, val, &i);
		else
			tmp = handle_unquoted_heredoc(*result_ptr, val, &i);
		if (!tmp)
			return (free(*result_ptr), NULL);
		*result_ptr = tmp;
	}
	return (*result_ptr);
}

char	*herdoc_parsing(char *val)
{
	char	*result;
	char	*tmp;

	if (!val)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	result = herdoc_parsing_sections(val, &result);
	if (!result)
		return (NULL);
	if (ft_strcmp(result, "\1") == 0)
	{
		free(result);
		result = ft_strdup("$");
		if (!result)
			return (NULL);
	}
	tmp = remove_char(result, '\1');
	return (free(result), tmp);
}
