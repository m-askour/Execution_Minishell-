/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:01:55 by maskour           #+#    #+#             */
/*   Updated: 2025/07/17 14:04:58 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while(env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	ft_putstr_fd_up(char *s, int fd)
{
	size_t	len;

	if (!s || fd < 0)
		return ;
	len = 0;
	while (s[len])
		len++;
	write(fd, s, len);
}

static int	count_cmd(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

static t_cmd	**convert_lit_arr(t_cmd *cmd, int cmd_count)
{
	int		i;
	t_cmd	*current;
	t_cmd	**cmd_arr;

	cmd_arr = malloc(sizeof(t_cmd *) * (cmd_count + 1));
	current = cmd;
	if (!cmd_arr)
		return (NULL);
	i = -1;
	while (++i < cmd_count)
	{
		cmd_arr[i] = current;
		current = current->next;
	}
	cmd_arr[cmd_count] = NULL;
	return (cmd_arr);
}

static void	execute_single(t_cmd **cmd, char **env,
				t_env **env_list, t_shell *shell_ctx)
{
	if (is_builtin((*cmd)->cmd[0]))
		*env_list = execut_bultin(cmd,*env_list, shell_ctx, 1);
	else
		execute_single_command(cmd, env_list, env, shell_ctx);
}

int	exicut(t_cmd **cmd, t_env **env_list, t_shell *shell_ctx)
{
	int		cmd_count;
	t_cmd	**cmd_arr;
	char	**env;

	if (!cmd || !*cmd || !env_list)
		return (1);
	env = convert(*env_list);
	if (!env)
		return (1);
	cmd_count = count_cmd(*cmd);
	if (cmd_count == 1)
		execute_single(cmd, env, env_list, shell_ctx);
	else
	{
		cmd_arr = convert_lit_arr(*cmd, cmd_count);
		if (!cmd_arr)
		{
			free_env(env);
			return (1);
		}
		execute_pipeline(cmd_arr, cmd_count, env, *env_list, shell_ctx);
		free(cmd_arr);
	}
	free_env(env);
	return (0);
}
