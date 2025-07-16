/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   storge_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:49:49 by ahari             #+#    #+#             */
/*   Updated: 2025/07/14 05:56:49 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	parse_redirections(t_cmd *cmd, t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (ft_isredirect(current->type))
		{
			if (!validate_redirection_token(current))
				return (0);
			if (!process_file_redirection(cmd, current))
				return (0);
			current = current->next;
		}
		current = current->next;
	}
	return (1);
}

static t_cmd	*parse_single_command(t_token **tokens)
{
	t_cmd	*cmd;
	t_token	*start;
	int		argc;
	int		redir_count;

	start = *tokens;
	argc = count_args(start);
	cmd = init_cmd();
	if (!cmd)
		return (NULL);
	cmd->cmd = malloc(sizeof(char *) * (argc + 1));
	if (!cmd->cmd)
		return (free(cmd), NULL);
	redir_count = count_redirections(start);
	cmd->files = malloc(sizeof(t_file) * (redir_count + 1));
	if (!cmd->files)
		return (free_array(cmd->cmd), free(cmd), NULL);
	if (!parse_arguments(cmd, *tokens))
		return (free(cmd->files), free(cmd), NULL);
	if (!parse_redirections(cmd, &start))
		return (free_array(cmd->cmd), free(cmd->files), free(cmd), NULL);
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
		*tokens = (*tokens)->next;
	cmd->next = NULL;
	return (cmd);
}

static int	handle_pipe_token(t_token **tokens,
	t_shell *shell_ctx, t_cmd *cmd_head)
{
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		if (!(*tokens)->next)
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			shell_ctx->exit_status = 258;
			free_cmd_list(cmd_head);
			return (0);
		}
		*tokens = (*tokens)->next;
	}
	return (1);
}

t_cmd	*parse_commands(t_token *tokens, t_shell *shell_ctx)
{
	t_cmd	*head;
	t_cmd	*current;
	t_cmd	*new_cmd;
	int		cmd_count;
	int		i;

	head = NULL;
	current = NULL;
	cmd_count = count_commands(tokens);
	i = 0;
	while (i < cmd_count)
	{
		new_cmd = parse_single_command(&tokens);
		if (!new_cmd)
			return (free_cmd_list(head), shell_ctx->exit_status = 258, NULL);
		if (!head)
			head = new_cmd;
		else
			current->next = new_cmd;
		current = new_cmd;
		if (!handle_pipe_token(&tokens, shell_ctx, head))
			return (NULL);
		i++;
	}
	return (head);
}
