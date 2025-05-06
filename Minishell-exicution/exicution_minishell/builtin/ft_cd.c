/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 18:49:15 by maskour           #+#    #+#             */
/*   Updated: 2025/05/06 11:41:47 by maskour          ###   ########.fr       */
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
    char *tmp;
    char *new_path;
    while(current)
    {
        if (!ft_strcmp(current->data_env,"PWD="))
        {
            tmp = ft_strjoin(key, "=");
            if (!tmp)
                 return;
            new_path = ft_strjoin(tmp, dest);
            free(tmp);
            if (!new_path)
                 return;
            free(current->data_env);
            current->data_env = new_path;
            return;
        }
        else if (!ft_strcmp(current->data_env,"PWD="))
        {
            tmp = ft_strjoin(key, "=");
            if (!tmp)
                 return;
            new_path = ft_strjoin(tmp, dest);
            free(tmp);     
            if (!new_path)
                 return;
            free(current->data_env);
            current->data_env = new_path;
            return;
        }
        
        current = current->next;
    }
}
void ft_cd(t_cmd **cmd, t_env *data_env)
{
    char *pwd_update = NULL;// STOR THE NEW PWD;
    char *oldpwd_update = NULL;//STORE THE LAST PWD
    char *path = NULL;
    int arg_count = count_cmd(cmd);
    t_cmd *cmd_path = *cmd;
    if (arg_count >1)
    {
        ft_putstr_fd_up("minishell: cd: too many arguments\n",2);
        return ;
    }
    // to save the current directory in oldpwd
    if (!getcwd(oldpwd_update, PATH_MAX))
    {
        perror("minishell: cd");
        return;
    }
    // char *key = search_env(data_env, "PWD=");
    
    if (chdir(cmd_path->cmd[1]) != 0)
        perror("no such directore");
    if (arg_count == 0 ||(cmd_path->cmd[1] && !ft_strcmp(cmd_path->cmd[1], "~")))
        {
            path = search_env(data_env,"HOME");
            if (!path)
            {
                ft_putstr_fd_up("minishell: cd: HOME not set\n",2);
                return ;
            }
        }
    else if (!ft_strcmp(cmd_path->cmd[1], "-"))
      {  
        path = search_env(data_env, "OLDPWD");
        if (!path)
        {
            ft_putstr_fd_up("minishell: cd: OLDPWD not set\n",2);
            return ;
        }
      }

    else
        path = cmd_path->cmd[1];
    
    //change directory
    if (chdir(path) != 0) //the chdir filed if don't return 0
    {
        ft_putstr_fd_up("minishell: cd: ",2);
        perror(path);
        return;
    }
        
    //update envrone=ment
    if (!getcwd(pwd_update, PATH_MAX))
    {
        perror("minishell: cd: getcwd error");
        return;
    }    
        //update the oldpwd;
    update_env_var(data_env, "OLDPWD", oldpwd_update);
    //update the pwd
    update_env_var(data_env, "PWD", pwd_update);

    //this to print new directory 
    if (cmd_path->cmd[1] && !ft_strcmp(cmd_path->cmd[1], "-"))
    {
        ft_putstr_fd_up(path, 1);
        ft_putstr_fd_up("\n",1);
    }   
}
