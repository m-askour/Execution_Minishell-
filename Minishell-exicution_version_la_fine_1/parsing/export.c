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

int	has_quotes(char *str)
{
	int				i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			return (1);
		if (str[i] == '$' || ft_isdigit(str[i]))
			return (3);
		if (str[i] == '=')
			break ;
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

int ft_parsexport(char *str)
{
	int qouted;
	int i;
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
char *ft_strremovechar(const char *str)
{
    int i = 0, j = 0;
    char *new_str;

    if (!str)
        return NULL;

    new_str = malloc(ft_strlen(str) + 1);
    if (!new_str)
        return NULL;

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
    return new_str;
}


int	is_export_assignment(t_token *head, t_token *current)
{
	t_token	*cmd_token;
	// int		qouted;
	char	*str;

	// qouted = ft_parsexport(current->value);
	str = ft_strremovechar(current->value); 
	cmd_token = get_cmd_token(head, current);
	if (cmd_token && (ft_strcmp(str, "export") == 0))
    {
        if (current->was_quoted == 1 || current->was_quoted == 3)
            return (2);
        if (ft_strchr(current->value, '='))
            return (1);
    }
	return (0);
}