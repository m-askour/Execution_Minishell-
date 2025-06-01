
#include "../minishell.h"

char *remove_quotes(char *str)
{
	int len;
	char *new_str;
	int start = 0;
	int end;

	if (!str)
		return NULL;
	len = ft_strlen(str);
	if (len == 0)
		return NULL;
	if (len == 2 && ((str[0] == '\'' && str[1] == '\'') || 
		(str[0] == '"' && str[1] == '"')))
		return NULL;
	end = len - 1;
	if (len > 1 && ((str[0] == '\'' && str[end] == '\'') || 
		(str[0] == '"' && str[end] == '"')))
	{
		start = 1;
		end = len - 2;
	}
	if (start > end)
		return NULL;
	new_str = malloc(end - start + 2);
	if (!new_str)
		return NULL;
	ft_strncpy(new_str, str + start, end - start + 1);
	new_str[end - start + 1] = '\0';
	return new_str;
}

t_cmd *unquote_cmd(t_cmd *cmd)
{
	int i = 0;
	if (!cmd)
		return NULL;
	while (cmd->cmd[i])
	{
		char *unquoted_cmd = remove_quotes(cmd->cmd[i]);
		if (!unquoted_cmd)
		{
			write(2, "minishell: ", 11);
			if (ft_strlen(cmd->cmd[i]) == 2 && 
				((cmd->cmd[i][0] == '\'' && cmd->cmd[i][1] == '\'') ||
				(cmd->cmd[i][0] == '"' && cmd->cmd[i][1] == '"')))
			{
				write(2, cmd->cmd[i], ft_strlen(cmd->cmd[i]));
				write(2, " not found\n", 12);
			}
			return NULL;
		}
		free(cmd->cmd[i]);
		cmd->cmd[i] = unquoted_cmd;
		i++;
	}
	return cmd;
}

t_cmd *unquote_file(t_cmd *cmd)
{
	int i = 0;
	if (!cmd)
		return NULL;
	while (i < cmd->file_count)
	{
		char *unquoted_file = remove_quotes(cmd->files[i].name);
		if (!unquoted_file)
		{
			write(2, "minishell: ", 11);
			if (ft_strlen(cmd->files[i].name) == 2 && 
				((cmd->files[i].name[0] == '\'' && cmd->files[i].name[1] == '\'') ||
				(cmd->files[i].name[0] == '"' && cmd->files[i].name[1] == '"')))
			{
				write(2, cmd->files[i].name, ft_strlen(cmd->files[i].name));
				write(2, ": not found\n", 12);
			}
			return NULL;
		}
		free(cmd->files[i].name);
		cmd->files[i].name = unquoted_file;
		i++;
	}
	return cmd;
}

t_cmd *unquote_cmd_list(t_cmd *cmd_head)
{
	t_cmd *current;
	if (!cmd_head)
		return NULL;	
	current = cmd_head;
	while (current != NULL)
	{
		if (!unquote_cmd(current))
			return NULL;
		if (!unquote_file(current))
			return NULL;
		current = current->next;
	}
	return cmd_head;
}
