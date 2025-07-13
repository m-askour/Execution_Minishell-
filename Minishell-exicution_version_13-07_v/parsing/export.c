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

static int is_var_exp_assignment(const char *str)
{
    const char *eq;
    const char *p;

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

static int is_exp(const char *str)
{
    int i = 0;
    int eq_pos = -1;

    if (!str || !*str)
        return (0);
    while (str[i])
	{
        if (str[i] == '=')
		{
            eq_pos = i;
            break;
        }
        i++;
    }
    if (eq_pos < 0 || str[eq_pos + 1] == '\0')
        return (0);
    if (str[eq_pos + 1] == '$')
        return (1);
    return (0);
}

int	is_export_assignment(t_token *head, t_token *current)
{
	t_token	*prev;

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
	if (ft_strcmp(prev->value, "export") == 0 && current->was_quoted != 0 \
		&& is_exp(current->value) != 1)
		return (1);
	if (prev && prev->was_quoted == 0)
	{
		if(is_var_exp_assignment(current->value) == 1)
			return (1);
	}
	return (0);
}
