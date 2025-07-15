/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 03:37:27 by ahari             #+#    #+#             */
/*   Updated: 2025/07/14 03:53:29 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*process_special_variable(char *cmd, int pos, t_shell *shell_ctx)
{
	char	*new_cmd;
	char	*expanded;

	if (cmd[pos + 1] == '?' || cmd[pos + 1] == '0' || ft_isdigit(cmd[pos + 1]))
	{
		new_cmd = handle_special_var(cmd, pos, shell_ctx);
		if (new_cmd)
		{
			expanded = found_env(new_cmd, NULL, shell_ctx);
			free(new_cmd);
			return (expanded);
		}
	}
	return (NULL);
}

static char	*process_env_variable(char *cmd, int pos,
	char **env, t_shell *shell_ctx)
{
	char	*new_cmd;
	char	*expanded;

	if (cmd[pos + 1] && is_char(cmd[pos + 1]))
	{
		new_cmd = handle_env_var(cmd, pos, env);
		if (new_cmd)
		{
			expanded = found_env(new_cmd, env, shell_ctx);
			free(new_cmd);
			return (expanded);
		}
	}
	return (NULL);
}

static char	*restore_dollar_signs(char *cmd)
{
	int		i;
	char	*result;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\x01')
			cmd[i] = '$';
		i++;
	}
	result = ft_strdup(cmd);
	return (result);
}

static char	*process_variables(char *cmd, char **env, t_shell *shell_ctx)
{
	int		pos;
	char	*result;

	pos = 0;
	while (cmd[pos])
	{
		if (should_expand_var(cmd, pos))
		{
			result = process_special_variable(cmd, pos, shell_ctx);
			if (result)
				return (result);
			result = process_env_variable(cmd, pos, env, shell_ctx);
			if (result)
				return (result);
		}
		pos++;
	}
	return (restore_dollar_signs(cmd));
}

char	*found_env(char *cmd, char **env, t_shell *shell_ctx)
{
	char	*preprocessed;
	char	*result;
	char	*expanded;

	if (!cmd)
		return (NULL);
	if (cmd[0] == '\1')
	{
		expanded = process_variables(cmd + 1, env, shell_ctx);
		if (!expanded)
			return (NULL);
		return (expanded);
	}
	preprocessed = replace_double_dollar(cmd);
	if (!preprocessed)
		return (NULL);
	result = process_variables(preprocessed, env, shell_ctx);
	free(preprocessed);
	return (result);
}
