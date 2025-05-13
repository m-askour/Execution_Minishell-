/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:43:32 by ahari             #+#    #+#             */
/*   Updated: 2025/05/13 12:15:00 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *ft_exit_status(char *cmd)
{
        return cmd;
}

char *found_env(char *cmd)
{
    int i = 0;
    int j = 0;
    char *new_cmd;
    
    if (!cmd)
        return NULL;
        
    while (cmd[j])
    {
        if (cmd[j] == '$' && cmd[j + 1] == '?')
        {
            char *exit_status = ft_exit_status(cmd);
            if (!exit_status)
                return NULL;
                
            new_cmd = malloc(ft_strlen(cmd) - 2 + ft_strlen(exit_status) + 1);
            if (!new_cmd)
            {
                free(exit_status);
                return NULL;
            }
            ft_strncpy(new_cmd, cmd, j);
            new_cmd[j] = '\0';
            ft_strcat(new_cmd, exit_status);
            ft_strcat(new_cmd, cmd + j + 2);
            free(exit_status);
            free(cmd);
            return new_cmd;
        }
        if (cmd[j] == '$')
        {
            i = j + 1;
            while (cmd[i] && (ft_isalnum(cmd[i]) || cmd[i] == '_'))
                i++;
            char *env = ft_substr(cmd, j + 1, i - j - 1);
            if (!env)
                return NULL;
            char *value = getenv(env);
            free(env);
            if (!value)
            {
                char *temp = malloc(j + ft_strlen(cmd + i) + 1);
                if (!temp)
                    return NULL;
                ft_strncpy(temp, cmd, j);
                temp[j] = '\0'; 
                ft_strcat(temp, cmd + i);
                free(cmd);
                return temp;
            }
            new_cmd = malloc(j + ft_strlen(value) + ft_strlen(cmd + i) + 1);
            if (!new_cmd)
                return NULL;
                
            ft_strncpy(new_cmd, cmd, j);
            new_cmd[j] = '\0';
            ft_strcat(new_cmd, value);
            ft_strcat(new_cmd, cmd + i);
            
            char *result = found_env(new_cmd);
            return result;
        }
        j++;
    }
    char *returned_cmd = ft_strdup(cmd);
    if (!returned_cmd)
    {
        free(cmd);
        return NULL;
    }
    return returned_cmd;
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