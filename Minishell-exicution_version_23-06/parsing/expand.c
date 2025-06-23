/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:43:32 by ahari             #+#    #+#             */
/*   Updated: 2025/06/20 22:12:43 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


static char *get_env(char **env, char *found_env)
{
	int i = 0;
	char *env_value;

	if (!env || !found_env)
		return NULL;
	while (env[i])
	{
		if (ft_strncmp(env[i], found_env, ft_strlen(found_env)) == 0 &&
			env[i][ft_strlen(found_env)] == '=')
		{
			env_value = ft_strdup(env[i] + ft_strlen(found_env) + 1);
			return env_value;
		}
		i++;
	}
	return NULL;
}

static char *ft_exit_status(t_shell *exit_stat)
{
	return ft_itoa(exit_stat->exit_status);
}


static int is_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static int get_var_name_length(char *str, int start)
{
	int len = 0;

	while (str[start + len] && is_char(str[start + len]))
		len++;
	return len;
}

static char *build_new_command(char *cmd, int pos, char *replacement, int skip_len)
{
	char *new_cmd;
	int new_len = ft_strlen(cmd) - skip_len + ft_strlen(replacement);

	new_cmd = malloc(new_len + 1);
	if (!new_cmd)
		return NULL;
	ft_strncpy(new_cmd, cmd, pos);
	new_cmd[pos] = '\0';
	ft_strcat(new_cmd, replacement);
	ft_strcat(new_cmd, cmd + pos + skip_len);
	return new_cmd;
}

static char *handle_spcial_var(char *cmd, int pos,t_shell *exited)
{
	char *replacement = NULL;
	char *new_cmd;

	if (cmd[pos + 1] == '?')
		replacement = ft_exit_status(exited);
	else if (cmd[pos + 1] == '0')
		replacement = ft_strdup("./minishell");
	else
		return NULL;
	if (!replacement)
		return NULL;
	new_cmd = build_new_command(cmd, pos, replacement, 2);
	free(replacement);
	return new_cmd;
}

static char *handle_env_var(char *cmd, int pos,char  **env)
{
	int var_start = pos + 1;
	int var_len = get_var_name_length(cmd, var_start);
	char *env_name, *env_value, *new_cmd;

	if (var_len == 0)
		return NULL;
	env_name = ft_substr(cmd, var_start, var_len);
	if (!env_name || !env)
		return NULL;
	env_value = get_env(env, env_name);
	free(env_name);
	if (env_value != NULL)
    	new_cmd = build_new_command(cmd, pos, env_value, var_len + 1);
	else
    	new_cmd = build_new_command(cmd, pos, "", var_len + 1);
	return new_cmd;
}

char *found_env(char *cmd, char **env,t_shell *shell_ctx)
{
	int pos = 0;
	char *new_cmd;

	if (!cmd)
		return NULL;
	while (cmd[pos])
	{
		if (cmd[pos] == '$'  && (pos == 0 || cmd[pos - 1] != '\\'))
		{
			if (cmd[pos + 1] == '\0' ||
				(!is_char(cmd[pos + 1]) && cmd[pos + 1] != '?' && cmd[pos + 1] != '0'))
			{
				pos++;
				continue;
			}
			if (cmd[pos + 1] == '?' || cmd[pos + 1] == '0')
				new_cmd = handle_spcial_var(cmd, pos, shell_ctx);
			else
				new_cmd = handle_env_var(cmd, pos, env);
			if (new_cmd)
				return (free(cmd), found_env(new_cmd, env, shell_ctx));
		}
		pos++;
	}
	char *result = ft_strdup(cmd);
	return (free(cmd), result);
}

static t_cmd *expand_file(t_cmd *cmd, t_shell *shell_ctx, char **env)
{
	int i = 0;

	if (!cmd)
		return NULL;
	while (i < cmd->file_count)
	{
		char *expanded = found_env(ft_strdup(cmd->files[i].name),env, shell_ctx);
		if (!expanded)
		{
			write(2, "minishell: ", 12);
			write(2, cmd->files[i].name, ft_strlen(cmd->files[i].name));
			write(2, ": ambiguous redirect\n", 22);
			return NULL;
		}
		free(cmd->files[i].name);
		cmd->files[i].name = expanded;
		i++;
	}
	return cmd;
}

t_cmd *expand_cmd_list(t_cmd *cmd_head, t_shell *shell_ctx, char **env)
{
	t_cmd *current = cmd_head;

	while (current)
	{
		if (!expand_file(current,shell_ctx, env))
			return NULL;
		current = current->next;
	}
	return cmd_head;
}
