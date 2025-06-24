/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:51:36 by maskour           #+#    #+#             */
/*   Updated: 2025/06/24 19:33:55 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int is_valid_identifier(const char *str)
{
	int i = 0;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return 0;
	while (str[i]) {
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return 0;
		i++;
	}
	return 1;
}

t_env *ft_unset(t_cmd **cmd, t_env *env, t_shell *shell_ctx)
{
	t_env *current;
	t_env *prev;
	t_env *to_free;
	int len;
	int i;
	int error;
	t_cmd *current_cmd;

	current_cmd = *cmd;
	i = 1;
	error = 0;
	while (current_cmd->cmd[i])
	{
		if (!is_valid_identifier(current_cmd->cmd[i]))
		{
			printf("unset: '%s': not a valid identifier\n", current_cmd->cmd[i]);
			error = 1;
			i++;
			continue;
		}
		len = ft_strlen(current_cmd->cmd[i]);
		current = env;
		prev = NULL;
		while (current != NULL)
		{
			if (!ft_strncmp(current->data_env, current_cmd->cmd[i], len) &&
				current->data_env[len] == '=')
			{
				to_free = current;
				if (prev == NULL)
					env = current->next;
				else
					prev->next = current->next;
				current = current->next;
				free(to_free->data_env);
				free(to_free);
			}
			else
			{
				prev = current;
				current = current->next;
			}
		}
		i++;
	}
	shell_ctx->exit_status = error;
	return (env);
}
