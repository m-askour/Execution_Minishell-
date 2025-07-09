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

int	ft_parsexport(char *str)
{
	int	qouted;
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	qouted = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			qouted = 1;
		i++;
	}
	return (qouted);
}

char	*ft_strremovechar(const char *str)
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
		if (str[i] != '\'' && str[i] != '\"')
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

static int	is_valid_export_token(t_token *cmd_token)
{
	char	*temp;
	int		valid;

	if (!cmd_token)
		return (0);
	temp = ft_strremovechar(cmd_token->value);
	valid = (temp && ft_strcmp(temp, "export") == 0 \
		&& cmd_token->was_quoted == 0);
	free(temp);
	return (valid);
}

static int	is_var_exp_assignment(const char *str)
{
	const char	*eq;
	const char	*p;

	p = str;
	eq = ft_strchr(str, '=');
	if (!eq || eq == str)
		return (0);
	if (ft_isdigit(*p))
		return (0);
	while (*p)
	{
		if (*p == '=')
			break ;
		if (*p == '\'' || *p == '"' || *p == '$')
			return (0);
		p++;
	}
	if (*(eq + 1) == '$' && *(eq + 2))
		return (1);
	return (0);
}

int	is_export_assignment(t_token *head, t_token *current)
{
	t_token	*cmd_token;
	t_token	*prev;

	prev = find_previous_token(head, current);
	if (!head || !current)
		return (0);
	if (prev && prev->was_quoted == 1 && ft_strlen(prev->value) == 0
		&& ft_strchr(current->value, '='))
		return (1);
	cmd_token = get_cmd_token(head, current);
	if (!is_valid_export_token(cmd_token))
		return (0);
	if (!is_var_exp_assignment(current->value))
		return (0);
	return (1);
}
