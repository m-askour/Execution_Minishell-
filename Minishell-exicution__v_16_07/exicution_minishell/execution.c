/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:01:55 by maskour           #+#    #+#             */
/*   Updated: 2025/07/15 19:22:49 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void free_env(char **env)
{
    if (!env)
        return ;
    int i = 0;
    while(env[i])
    {
        free(env[i]);
        i++;
    }
    free(env);
}

char **convert(t_env *env_list)
{
    int count = 0;
    int i = -1;
    char **env_arry;
    t_env *current = env_list;
    if (!env_list)
    {
        env_arry = malloc(sizeof(char *) * 1);
        if (!env_arry)
            return NULL;
        env_arry[0] = NULL;
        return env_arry;
    }
    while (current)
    {
        count++;
        current = current->next;
    }
    env_arry = malloc (sizeof(char *) * (count + 1));
    if (!env_arry)
        return (NULL);
    current = env_list;
    while (++i < count && current)
    {
        env_arry[i] = ft_strdup(current->data_env); 
        if (!env_arry[i])
        {
            while (--i >= 0)
                free(env_arry[i]);
            free(env_arry);
            return (NULL);
        }
        current = current->next;
    }
    env_arry[count] = NULL;
    return (env_arry);
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

static int count_cmd(t_cmd *cmd)
{
    int count;
    
    count = 0;
    while (cmd)
    {
        count++;
        cmd = cmd->next;
    }
    return (count);
}

static t_cmd **convert_lit_arr(t_cmd *cmd, int cmd_count)
{
        t_cmd    *current = cmd;
        t_cmd **cmd_arr = malloc(sizeof(t_cmd *) * (cmd_count + 1));
        if (!cmd_arr)
            return (NULL);
        int i = -1;
        while ( ++i < cmd_count) 
        {
            cmd_arr[i] = current;
            current = current->next;
        }
        cmd_arr[cmd_count] = NULL;
        return (cmd_arr);
}

static void execute_single(t_cmd **cmd, char **env, t_env **env_list, t_shell *shell_ctx)
{
    if (is_builtin((*cmd)->cmd[0]))
        *env_list = execut_bultin(cmd,*env_list, shell_ctx, 1);
    else
        execute_single_command(cmd,env_list, env, shell_ctx);
}

int exicut(t_cmd **cmd, t_env **env_list, t_shell *shell_ctx)
{
    int cmd_count;
    t_cmd **cmd_arr;
    char **env;

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
        if (!cmd_arr) {
            free_env(env);
            return (1);
        }
        execute_pipeline(cmd_arr, cmd_count, env, *env_list,shell_ctx);
        free(cmd_arr);
    }
    free_env(env);
    return (0);
}
