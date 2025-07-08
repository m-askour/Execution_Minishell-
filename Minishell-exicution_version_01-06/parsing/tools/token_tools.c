// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   token_tools.c                                      :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/06/27 21:11:58 by ahari             #+#    #+#             */
// /*   Updated: 2025/06/27 21:12:28 by ahari            ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "../../minishell.h"

int has_quotes(char *str)
{
	int i;

    i = 0;
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

t_token	*get_cmd_token(t_token *head, t_token *current)
{
	t_token *temp;
	t_token *cmd_token;

    temp = head;
    cmd_token = NULL;
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

int count_herdoc(t_cmd *cmd)
{
    int count = 0;
    int i= 0;
    while (i < cmd->file_count) 
    {
        if (cmd->files->type == TOKEN_HEREDOC)
                count++;
        i++;
    }
    return count;
}
