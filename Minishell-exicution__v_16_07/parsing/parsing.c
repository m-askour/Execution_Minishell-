/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 03:04:25 by ahari             #+#    #+#             */
/*   Updated: 2025/07/14 05:04:36 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		if (array[i])
			free(array[i]);
		i++;
	}
	free(array);
}

t_token	*find_previous_token(t_token *head, t_token *target)
{
	t_token	*current;

	if (!head || head == target)
		return (NULL);
	current = head;
	while (current->next && current->next != target)
		current = current->next;
	if (current->next == target)
		return (current);
	else
		return (NULL);
}

t_token	*check_quoted_process_tokens(t_token *head,
	char *str, t_env_list *env_list)
{
	t_token	*current;
	t_token	*prev;
	char	*new_val;

	current = head;
	while (current)
	{
		prev = find_previous_token(head, current);
		if (prev && prev->type == TOKEN_HEREDOC)
		{
			new_val = herdoc_parsing(current->value);
			if (!new_val)
				return (free(env_list), free_tokens(head, str), NULL);
			free(current->value);
			current->value = new_val;
		}
		else
		{
			if (process_token(current, &head, env_list) == 0)
				return (free(env_list), free_tokens(head, str), NULL);
		}
		current = current->next;
	}
	return (head);
}

t_token	*check_quoted(char *str, t_shell *shell_ctx, char **env_tab)
{
	t_token		*head;
	t_env_list	*env_list;

	env_list = malloc(sizeof(t_env_list));
	if (!env_list)
		return (NULL);
	env_list->env_table = env_tab;
	env_list->exit_status = shell_ctx;
	head = string_tokens(str, shell_ctx);
	if (!head)
	{
		free(env_list);
		if (head)
			free_tokens(head, str);
		return (NULL);
	}
	head = check_quoted_process_tokens(head, str, env_list);
	free(env_list);
	return (head);
}
