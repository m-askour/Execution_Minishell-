/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:51:36 by maskour           #+#    #+#             */
/*   Updated: 2025/07/09 17:22:27 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_valid_identifier(const char *str)
{
	int i;

	i = 0;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return 0;
	while (str[i]) {
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return 0;
		i++;
	}
	return 1;
}

static t_env	*remove_env_var(t_env *env, char *var)
{
	t_env *to_free;
	t_env *prev;
	t_env *current;

	current = env;
	prev = NULL;
	while (current != NULL)
	{
		if (!ft_strncmp(current->data_env, var, ft_strlen(var)))
		{
			to_free = current;
			if (prev == NULL) {env = current->next;}
			else {prev->next = current->next;}
			current = current->next;
			free(to_free->data_env);
			free(to_free);
			continue;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
	return (env);
}

t_env	*ft_unset(t_cmd **cmd, t_env *env, t_shell *shell_ctx)
{
	int i;
	int error;
	t_cmd *current_cmd;

	i = 1;
	error = 0;
	current_cmd = *cmd;
	while (current_cmd->cmd[i])
	{
		if (!is_valid_identifier(current_cmd->cmd[i]))
		{
			printf("unset: '%s': not a valid identifier\n", current_cmd->cmd[i]);
			error = 1;
			i++;
			continue;
		}
		env = remove_env_var(env, current_cmd->cmd[i]);
		i++;
	}
	shell_ctx->exit_status = error;
	return (env);
}
