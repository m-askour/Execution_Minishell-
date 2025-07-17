/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 22:05:02 by ahari             #+#    #+#             */
/*   Updated: 2025/07/14 06:19:42 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env(char **env, char *found_env)
{
	int		i;
	char	*env_value;

	i = 0;
	if (!env || !found_env)
		return (NULL);
	while (env[i])
	{
		if (ft_strncmp(env[i], found_env, ft_strlen(found_env)) == 0 && \
			env[i][ft_strlen(found_env)] == '=')
		{
			env_value = ft_strdup(env[i] + ft_strlen(found_env) + 1);
			return (env_value);
		}
		i++;
	}
	return (NULL);
}

char	*handle_special_var(char *cmd, int pos, t_shell *exited)
{
	char	*replacement;
	char	*new_cmd;

	replacement = NULL;
	if (cmd[pos + 1] == '?')
		replacement = ft_exit_status(exited);
	else if (cmd[pos + 1] == '0')
		replacement = ft_strdup("./minishell");
	else if (ft_isdigit(cmd[pos + 1]))
		replacement = ft_strdup("");
	else
		return (NULL);
	if (!replacement)
		return (NULL);
	new_cmd = build_new_command(cmd, pos, replacement, 2);
	free(replacement);
	return (new_cmd);
}

char	*build_new_command(char *cmd, int pos, char *replacement, int skip_len)
{
	char	*new_cmd;
	int		new_len;

	new_len = ft_strlen(cmd) - skip_len + ft_strlen(replacement);
	new_cmd = malloc(new_len + 1);
	if (!new_cmd)
		return (NULL);
	ft_strncpy(new_cmd, cmd, pos);
	new_cmd[pos] = '\0';
	ft_strcat(new_cmd, replacement);
	ft_strcat(new_cmd, cmd + pos + skip_len);
	return (new_cmd);
}

char	*restore_dollar_signs(char *cmd)
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
