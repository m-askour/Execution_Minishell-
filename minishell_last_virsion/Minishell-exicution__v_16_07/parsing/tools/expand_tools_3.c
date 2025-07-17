/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tools_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 03:45:53 by ahari             #+#    #+#             */
/*   Updated: 2025/07/14 06:19:05 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_strspaces(char *str)
{
	int	i;

	if (!str)
		return (1);
	i = 0;
	while (str[i])
	{
		if (!ft_isspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}

char	*replace_double_dollar(char *cmd)
{
	int		len;
	char	*new_cmd;
	int		i;
	int		j;

	len = ft_strlen(cmd);
	new_cmd = malloc(len + 1);
	i = 0;
	j = 0;
	if (!new_cmd)
		return (NULL);
	while (cmd[i])
	{
		if (cmd[i] == '$' && cmd[i + 1] == '$')
		{
			new_cmd[j++] = '\x01';
			i += 2;
		}
		else
			new_cmd[j++] = cmd[i++];
	}
	new_cmd[j] = '\0';
	return (new_cmd);
}

int	should_expand_var(char *cmd, int pos)
{
	if (cmd[pos] != '$')
		return (0);
	if (pos > 0 && cmd[pos - 1] == '\\')
		return (0);
	return (1);
}

char	*safe_strcat_heredoc(char *dest, char *src)
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

char	*process_quoted_heredoc(char *val, int *i)
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
