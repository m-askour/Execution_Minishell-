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

static int	has_quotes(char *str)
{
	int	i = 0;

	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

static t_token	*get_cmd_token(t_token *head, t_token *current)
{
	t_token	*temp = head;
	t_token	*cmd_token = NULL;

	while (temp && temp != current)
	{
		if (temp->type == TOKEN_PIPE || temp->type == TOKEN_SEMICOLON)
			cmd_token = NULL;
		else if (temp->type == TOKEN_WORD && !cmd_token)
			cmd_token = temp;
		temp = temp->next;
	}
	return (cmd_token);
}


int	is_export_assignment(t_token *head, t_token *current)
{
	t_token	*cmd_token;

	cmd_token = get_cmd_token(head, current);
	if (cmd_token && ft_strcmp(cmd_token->value, "export") == 0)
	{
		if (has_quotes(current->value))
			return (2);
		if (ft_strchr(current->value, '='))
			return (1);
	}
	return (0);
}