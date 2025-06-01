/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:51:36 by maskour           #+#    #+#             */
/*   Updated: 2025/05/26 17:14:37 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_env *ft_unset(t_cmd **cmd, t_env *env)
{
    t_env *current;
    t_env *prev;
    t_env *to_free;
    int len;
    int i = 1;
    t_cmd *current_cmd = *cmd;
    while (current_cmd->cmd[i])
    {
        len = ft_strlen(current_cmd->cmd[i]);
        current = env;
        prev = NULL;
        while (current != NULL)
        {
            if(!ft_strncmp(current->data_env, current_cmd->cmd[i], len) && \
                    current->data_env[len] == '=')
            {
                to_free = current;
                if (prev == NULL)
                    env = current->next;
                else
                    prev->next = current->next;
                current = current->next;
                free(to_free->data_env);
                free(to_free);
            }
            else 
            {
                prev = current;
                current = current->next;
            }
        }
        i++;
    }
    return (env);
}
