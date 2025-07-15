/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   storge_tokens_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 05:09:37 by ahari             #+#    #+#             */
/*   Updated: 2025/07/14 05:52:06 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	process_word_token(t_cmd *cmd, t_token *current, int *arg_i)
{
	char	*tmp;

	if (current->type == TOKEN_WORD && \
		((ft_strcmp(current->value, "\2") != 0 && \
		!(ft_strspaces(current->value) && current->was_quoted != 1)) || \
		(ft_strcmp(current->value, "\2") == 0 && current->was_quoted == 1)))
	{
		tmp = ft_strdup(current->value);
		if (!tmp)
			return (0);
		cmd->cmd[*arg_i] = remove_char(tmp, '\2');
		free(tmp);
		if (!cmd->cmd[*arg_i])
			return (free_cmd_args(cmd->cmd, *arg_i), 0);
		(*arg_i)++;
	}
	return (1);
}

static int	process_redirect_token(t_cmd *cmd, t_token *current, int arg_i)
{
	if (ft_isredirect(current->type))
	{
		if (!current->next || current->next->type != TOKEN_WORD)
		{
			print_syntax_error(current);
			if (cmd->cmd)
				free_cmd_args(cmd->cmd, arg_i);
			return (0);
		}
	}
	return (1);
}

int	parse_arguments(t_cmd *cmd, t_token *tokens)
{
	int		arg_i;
	t_token	*current;

	arg_i = 0;
	current = tokens;
	if (!cmd->cmd)
		return (0);
	while (current && current->type != TOKEN_PIPE)
	{
		if (!process_word_token(cmd, current, &arg_i))
			return (0);
		if (ft_isredirect(current->type))
		{
			if (!process_redirect_token(cmd, current, arg_i))
				return (0);
			current = current->next;
		}
		current = current->next;
	}
	cmd->cmd[arg_i] = NULL;
	tokens = current;
	return (1);
}

int	process_file_redirection(t_cmd *cmd, t_token *current)
{
	t_file	*file;
	char	*tmp;

	file = init_mfile();
	if (!file)
		return (0);
	tmp = ft_strdup(current->next->value);
	if (!tmp)
		return (free(file), 0);
	file->name = remove_char(tmp, '\2');
	if (!file->name)
		return (free(file), 0);
	file->type = current->type;
	file->check_expand = current->next->was_quoted;
	cmd->files[cmd->file_count++] = *file;
	free(tmp);
	free(file);
	return (1);
}
