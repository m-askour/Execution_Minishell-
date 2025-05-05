/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 18:48:55 by maskour           #+#    #+#             */
/*   Updated: 2025/05/05 15:46:41 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


int is_builtin(char *command)
{
    if (!command)
        return (0);
    return (ft_strcmp(command, "cd") == 0 ||\
        ft_strcmp(command, "echo") == 0 ||\
        ft_strcmp(command, "exit") == 0 ||\
        ft_strcmp(command, "export") == 0 ||\
        ft_strcmp(command, "unset") == 0 ||\
        ft_strcmp(command, "env") == 0 ||\
        ft_strcmp(command, "pwd") == 0);
}

void *execut_bultin(t_cmd **cmd, char **env)
{
    t_env *env_list = file_inv(env);

    if (ft_strcmp(cmd[0]->cmd[0], "cd") == 0)
        ft_cd(cmd, env_list);
    if (ft_strcmp(cmd[0]->cmd[0], "echo") == 0)
        ft_echo(cmd);
    if(ft_strcmp(cmd[0]->cmd[0], "exit") == 0)
        ft_exit(cmd);
    if (ft_strcmp(cmd[0]->cmd[0], "export") == 0)
        ft_export(cmd, env_list);
    if (ft_strcmp(cmd[0]>cmd[0], "unset") == 0)
        ft_unset(cmd);
    if (ft_strcmp(cmd[0]->cmd[0], "env") == 0)
        ft_env(env_list);
    if (ft_strcmp(cmd[0]->cmd[0], "pwd") == 0)
        ft_pwd();
    return ;
}