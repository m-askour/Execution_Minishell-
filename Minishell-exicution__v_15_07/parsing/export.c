/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_with_quotes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: username <username@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by username          #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by username         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_exp_var(const char *str)
{
	const char	*eq;
	const char	*p;

	p = str;
	eq = ft_strchr(str, '=');
	if (!eq || eq == str)
		return (0);
	if (ft_isdigit(*p) || *p == '\'' || *p == '"')
		return (0);
	while (*p && p < eq)
	{
		if (*p == '\'' || *p == '"' || *p == '$')
			return (0);
		p++;
	}
	if (*(eq + 1) == '$')
		return (1);
	if (*(eq + 1) == '\'' || *(eq + 1) == '"')
	{
		if (*(eq + 2) == '$')
			return (1);
		return (1);
	}
	return (0);
}

static int	is_exp(const char *str)
{
	int	i;
	int	eq_pos;

	eq_pos = -1;
	i = 0;
	if (!str || !*str)
		return (0);
	while (str[i])
	{
		if (str[i] == '=')
		{
			eq_pos = i;
			break ;
		}
		i++;
	}
	if (eq_pos < 0 || str[eq_pos + 1] == '\0')
		return (0);
	if (str[eq_pos + 1] == '$')
		return (1);
	return (0);
}

static char	*ft_strremovechar(const char *str)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	new_str = malloc(ft_strlen(str) + 1);
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		if (str[i] != '\'' || str[i] != '\"')
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

int	is_export_assignment(t_token *head, t_token *current)
{
	t_token	*prev;
	char	*tmp;

	if (!head || !current)
		return (0);
	prev = find_previous_token(head, current);
	if (!prev)
	{
		if (current->was_quoted != 0)
			return (1);
		else
			return (0);
	}
	tmp = ft_strremovechar(prev->value);
	if (!tmp)
		return (0);
	if (ft_strcmp(tmp, "export") == 0 && current->was_quoted == 0 \
		&& is_exp(current->value) != 1)
		return (free(tmp), 1);
	if (ft_strcmp(tmp, "export") == 0 && current->was_quoted != 0 \
		&& is_exp(current->value) != 1)
		return (free(tmp), 1);
	if (prev && prev->was_quoted == 0 && is_exp_var(current->value) == 1)
		return (free(tmp), 1);
	return (free(tmp), 0);
}
