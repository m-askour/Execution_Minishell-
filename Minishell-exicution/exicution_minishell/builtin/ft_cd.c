/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 18:49:15 by maskour           #+#    #+#             */
/*   Updated: 2025/05/05 15:43:18 by maskour          ###   ########.fr       */
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
        if (!ft_strcmp(current->data_env,"PWD"))
        {
            if (!ft_strcmp(key,"OLDPWD"))
                dest = current->data_env; 
            else    
            {
                new_path = ft_strjoin(key, dest);
                if (!new_path)
                    return;
                free(current->data_env);
                current->data_env = new_path;
                return;
            }
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
    if (arg_count > 2)
    {
        ft_putstr_fd_up("minishell: cd: too many arguments\n",2);
        return ;
    }
    if (!getcwd(oldpwd_update, PATH_MAX))
    {
        perror("minishell: cd");
        return;
    }
    char *key = serch(data_env, "PWD=");
    
    if (chdir(cmd) != 0)
        perror("no such directore");
    if (arg_count == 1 || cmd[0]->cmd[1] == "~")
        path = search_env(data_env,"HOME");
    else if (!ft_strcmp(cmd[0]->cmd[0], "-"))
        path = search_env(data_env, "OLDPWD");
    else if (!ft_strcmp(cmd[1], "/"))
        path = "/";
    else if (!ft_strcmp(cmd[1],".."))
        path = "..";
    else
        path = cmd[0]->cmd[1];
    
    //change directory
    if (chdir(path))
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
    if (cmd[0]->cmd[1] && !ft_strcmp(cmd[0]->cmd[1], "-"))
    {
        ft_putstr_fd_up(path, 1);
        ft_putstr_fd_up("\n",1);
    }   
}
