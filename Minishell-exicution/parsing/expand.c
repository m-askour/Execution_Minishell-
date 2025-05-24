/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:43:32 by ahari             #+#    #+#             */
/*   Updated: 2025/05/24 20:45:51 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *ft_exit_status(char *cmd)
{
	return cmd;
}

int is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int get_var_name_length(char *str, int start)
{
	int len = 0;
	int i = start;
	
	while (str[i] && is_valid_var_char(str[i]))
	{
		len++;
		i++;
	}
	return len;
}

char *create_literal_var(char *cmd, int var_start, int var_len)
{
	char *literal = malloc(var_len + 2);
	if (!literal)
		return NULL;
	
	literal[0] = '$';
	ft_strncpy(literal + 1, cmd + var_start, var_len);
	literal[var_len + 1] = '\0';
	return literal;
}

char *build_new_command(char *cmd, int pos, char *replacement, int skip_len)
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

char *handle_special_var(char *cmd, int pos)
{
	char *replacement = NULL;
	char *new_cmd;
	
	if (cmd[pos + 1] == '?')
		replacement = ft_exit_status(cmd);
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

char *handle_env_var(char *cmd, int pos)
{
	int var_start = pos + 1;
	int var_len = get_var_name_length(cmd, var_start);
	char *new_cmd;
	if (var_len == 0)
		return NULL;
	char *env_name = ft_substr(cmd, var_start, var_len);
	if (!env_name)
		return NULL;
	char *env_value = getenv(env_name);
	free(env_name);
	if (!env_value)
		new_cmd = build_new_command(cmd, pos, "", var_len + 1);
	else
		new_cmd = build_new_command(cmd, pos, env_value, var_len + 1);
	return new_cmd;
}

char *found_env(char *cmd)
{
	int pos = 0;
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
		free(cmd);
		return temp;
	}
	while (cmd[pos])
	{
		if (cmd[pos] == '"' && (pos == 0 || cmd[pos - 1] != '\\'))
			in_double_quotes = !in_double_quotes;
		if (cmd[pos] == '$' && (pos == 0 || cmd[pos - 1] != '\\'))
		{
			int i = pos + 1;
			while (cmd[i] == '"')
				i++;
			if (cmd[i] == '\0' || (!is_valid_var_char(cmd[i]) && 
					cmd[i] != '?' && cmd[i] != '0' && cmd[i] != '$'))
			{
				pos++;
				continue;
			}
			if (cmd[i] == '?' || cmd[i] == '0' || cmd[i] == '$')
			{
				new_cmd = handle_special_var(cmd, pos);
				if (new_cmd)
				{
					free(cmd);
					return found_env(new_cmd);
				}
			}
			// Handle normalization like $""""USER → $USER chof hadi mohammed
			int start = i;
			while (ft_isalnum(cmd[i]) || cmd[i] == '_')
				i++;
			int var_len = i - start;
			if (var_len > 0)
			{
				int before_len = pos;
				int after_len = ft_strlen(cmd + i);
				int total_len = before_len + 1 + var_len + after_len + 1;
				char *result = malloc(total_len);
				if (!result)
				{
					free(cmd);
					return NULL;
				}
				ft_strncpy(result, cmd, before_len);
				result[before_len] = '$';
				ft_strncpy(result + before_len + 1, cmd + start, var_len);
				ft_strcpy(result + before_len + 1 + var_len, cmd + i);
				result[total_len - 1] = '\0';
				free(cmd);
				return result;
			}
			new_cmd = handle_env_var(cmd, pos);
			if (new_cmd)
			{
				free(cmd);
				return found_env(new_cmd);
			}
		}
		pos++;
	}
	char *result = ft_strdup(cmd);
	free(cmd);
	return result;
}

t_cmd *expand_cmd(t_cmd *cmd)
{
	int i = 0;
	if (!cmd)
		return NULL;
	while (cmd->cmd[i])
	{
		char *expanded_cmd = found_env(ft_strdup(cmd->cmd[i]));
		if (!expanded_cmd)
			return NULL;
		free(cmd->cmd[i]);
		cmd->cmd[i] = expanded_cmd;
		i++;
	}
	return cmd;
}

t_cmd *expand_file(t_cmd *cmd)
{
	int i = 0;
	if (!cmd)
		return NULL;
	while (i < cmd->file_count)
	{
		char *expanded_file = found_env(ft_strdup(cmd->files[i].name));
		if (!expanded_file)
		{
			write(2, "minishell: ", 12);
			write(2, cmd->files[i].name, strlen(cmd->files[i].name));
			write(2, ": ambiguous redirect\n", 22);
			return NULL;
		}
		free(cmd->files[i].name);
		cmd->files[i].name = expanded_file;
		i++;
	}
	return cmd;
}

t_cmd *expand_cmd_list(t_cmd *cmd_head)
{
	t_cmd *current;
	if (!cmd_head)
		return NULL;
	current = cmd_head;
	while (current != NULL)
	{
		if (!expand_cmd(current))
			return NULL;
		if (!expand_file(current))
			return NULL;
		current = current->next;
	}
	return cmd_head;
}