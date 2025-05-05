/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:51:36 by maskour           #+#    #+#             */
/*   Updated: 2025/05/02 21:25:25 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void ft_unset(t_cmd **cmd, t_env *env)
{
    t_env *current = env;
    int i = 0;
    while (cmd[0]->cmd[i])
    {
        while (current != NULL || current->next != NULL)
        {
            if(!ft_strcmp(current->data_env, cmd[0]->cmd[i]))
                free(current->data_env);
            current = current->next;
        }
        i++;
    }
}