/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 18:49:15 by maskour           #+#    #+#             */
/*   Updated: 2025/05/14 16:49:19 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
//UPDATE  put the PWD
//UPDate OLDPWD
//GO TO THE DIRECTION USE GETDR AND GETENV FUNCTIONS
static int count_cmd(t_cmd **cmd)
{
    int i= 0;
    while (cmd[0]->cmd[i])
    {
        if (cmd[0]->cmd[0] == cmd[0]->cmd[i+1])
            return (i);
        i++;
    }
    return (0);
}
static void update_env_var(t_env *data_env, char *key, char *dest)
{
    t_env *current = data_env;
    char *new_path;
    while(current)
    {
        if (ft_strstr(current->data_env,key) != NULL)
        {
            new_path = ft_strjoin(key, dest);
            if (!new_path)
                 return;
            free(current->data_env);
            current->data_env = new_path;
            return;
        }
        current = current->next;
    }
}

t_env *ft_cd(t_cmd **cmd, t_env *data_env)
{
    char pwd_update[PATH_MAX]; // Buffer for new PWD
    char oldpwd_update[PATH_MAX]; // Buffer for old PWD
    char *path = NULL;
    int arg_count = count_cmd(cmd);
    t_cmd *cmd_path = *cmd;
    if (!cmd_path->cmd[1])
        return (data_env);
    if (arg_count > 1)
    {
        ft_putstr_fd_up("minishell: cd: too many arguments\n", 2);
        return (data_env);
    }

    // Save current directory in oldpwd_update
    if (!getcwd(oldpwd_update, PATH_MAX))
    {
        perror("minishell: cd");
        return (data_env);
    }

    // Determine path to change to
    if (arg_count == 0 && (cmd_path->cmd[1] && !ft_strcmp(cmd_path->cmd[1], "~")))
        {
            path = search_env(data_env,"HOME");
            printf("%s\n", path);
            if (!path)
            {
                ft_putstr_fd_up("minishell: cd: HOME not set\n",2);
                return (data_env);
            }
        }
    else if (!ft_strcmp(cmd_path->cmd[1], "-") && cmd_path->cmd[1])
      {  
        path = search_env(data_env, "OLDPWD");
        if (!path)
        {
            ft_putstr_fd_up("minishell: cd: OLDPWD not set\n",2);
            return (data_env);
        }
      }

    else
        path = cmd_path->cmd[1];

    // Change directory
    if (chdir(path) != 0)
    {
        ft_putstr_fd_up("minishell: cd: ", 2);
        perror(path);
        return(data_env);
    }

    // Update environment
    if (!getcwd(pwd_update, PATH_MAX))
    {
        perror("minishell: cd: getcwd error");
        return(data_env);
    }

    // Update the oldpwd
    update_env_var(data_env, "OLDPWD=", oldpwd_update);
    // Update the pwd
    update_env_var(data_env, "PWD=", pwd_update);
    // Print new directory if cd -
    if (cmd_path->cmd[1] && !ft_strcmp(cmd_path->cmd[1], "-"))
    {
        ft_putstr_fd_up(pwd_update, 1); // Should print new directory (OLDPWD)
        ft_putstr_fd_up("\n", 1);
    }
    return (data_env);
}