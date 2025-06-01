/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:43:32 by ahari             #+#    #+#             */
/*   Updated: 2025/05/28 23:26:12 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char *ft_exit_status(t_shell *exit_stat)
{
	return ft_itoa(exit_stat->exit_status);
}

static int is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static int get_var_name_length(char *str, int start)
{
	int len = 0;

	while (str[start + len] && is_valid_var_char(str[start + len]))
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
	else if (cmd[pos + 1] == '$')
		replacement = ft_itoa(getpid());
	else
		return NULL;
	if (!replacement)
		return NULL;
	new_cmd = build_new_command(cmd, pos, replacement, 2);
	free(replacement);
	return new_cmd;
}

static char *handle_env_var(char *cmd, int pos)
{
	int var_start = pos + 1;
	int var_len = get_var_name_length(cmd, var_start);
	char *env_name, *env_value, *new_cmd;

	if (var_len == 0)
		return NULL;
	env_name = ft_substr(cmd, var_start, var_len);
	if (!env_name)
		return NULL;
	env_value = getenv(env_name);
	free(env_name);
	new_cmd = build_new_command(cmd, pos, env_value ? env_value : "", var_len + 1);
	return new_cmd;
}

static char *found_env(char *cmd, t_shell *shell_ctx)
{
	int pos = 0;
	int in_single_quotes = 0;
	int in_double_quotes = 0;
	char *new_cmd;

	if (!cmd)
		return NULL;
	if (cmd[0] == '\'' && cmd[ft_strlen(cmd) - 1] == '\'')
	{
		int len = ft_strlen(cmd);
		char *temp = malloc(len - 1);
		if (!temp)
			return NULL;
		ft_strncpy(temp, cmd + 1, len - 2);
		temp[len - 2] = '\0';
		return (free(cmd),temp);
	}
	while (cmd[pos])
	{
		if (cmd[pos] == '\'' && !in_double_quotes && (pos == 0 || cmd[pos - 1] != '\\'))
			in_single_quotes = !in_single_quotes;
		else if (cmd[pos] == '"' && !in_single_quotes && (pos == 0 || cmd[pos - 1] != '\\'))
			in_double_quotes = !in_double_quotes;
		if (cmd[pos] == '$' && !in_single_quotes && (pos == 0 || cmd[pos - 1] != '\\'))
		{
			if (cmd[pos + 1] == '\0' ||
				(!is_valid_var_char(cmd[pos + 1]) &&
				cmd[pos + 1] != '?' && cmd[pos + 1] != '0' && cmd[pos + 1] != '$'))
			{
				pos++;
				continue;
			}
			if (cmd[pos + 1] == '?' || cmd[pos + 1] == '0' || cmd[pos + 1] == '$')
				new_cmd = handle_spcial_var(cmd, pos, shell_ctx);
			else
				new_cmd = handle_env_var(cmd, pos);
			if (new_cmd)
				return (free(cmd), found_env(new_cmd, shell_ctx));
		}
		pos++;
	}
	char *result = ft_strdup(cmd);
	return (free(cmd), result);
}

static t_cmd *expand_cmd(t_cmd *cmd, t_shell *shell_ctx)
{
	int i = 0;

	if (!cmd)
		return NULL;
	while (cmd->cmd[i])
	{
		char *expanded = found_env(ft_strdup(cmd->cmd[i]),shell_ctx);
		if (!expanded)
			return NULL;
		free(cmd->cmd[i]);
		cmd->cmd[i] = expanded;
		i++;
	}
	return cmd;
}

static t_cmd *expand_file(t_cmd *cmd, t_shell *shell_ctx)
{
	int i = 0;

	if (!cmd)
		return NULL;
	while (i < cmd->file_count)
	{
		char *expanded = found_env(ft_strdup(cmd->files[i].name),shell_ctx);
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

t_cmd *expand_cmd_list(t_cmd *cmd_head, t_shell *shell_ctx)
{
	t_cmd *current = cmd_head;

	while (current)
	{
		if (!expand_cmd(current,shell_ctx))
			return NULL;
		if (!expand_file(current,shell_ctx))
			return NULL;
		current = current->next;
	}
	return cmd_head;
}
