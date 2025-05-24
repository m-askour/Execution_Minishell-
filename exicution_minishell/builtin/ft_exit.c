/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 19:01:03 by maskour           #+#    #+#             */
/*   Updated: 2025/05/06 10:26:17 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void ft_exit(t_cmd **cmd)
{
    int status = 0;
    int i = 0;
    t_cmd *current_cmd = *cmd;
    if (!current_cmd->cmd[1])
        exit(0);
    else
    {
        while(current_cmd->cmd[1][i])
        {
            
        if (!ft_isdigit(current_cmd->cmd[1][i]))
        {
            ft_putstr_fd_up("numeric argument required",2);
            exit(2);
        }
        i++;
        }
    
     if (current_cmd->cmd[2] != NULL)
    {
        ft_putstr_fd_up("minishell: exit: too many arguments\n",2);
        return ;
    }
        status = ft_atoi(current_cmd->cmd[1]) % 256;
        if (status < 0)
            status +=256;
        exit(status);
    }
}
