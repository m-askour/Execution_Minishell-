/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 19:01:03 by maskour           #+#    #+#             */
/*   Updated: 2025/06/19 13:08:45 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void ft_exit(t_cmd **cmd, t_shell *shell)
{
    int status = shell->exit_status;
    int i = 0;
    t_cmd *current_cmd = *cmd;
    printf("exit\n");
    if (!current_cmd->cmd[1])
        exit(shell->exit_status);
    if (current_cmd->cmd[1][i] == '+' || current_cmd->cmd[1][i] == '-')
        i++; 
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
    printf("exit");
    exit(status);
}
