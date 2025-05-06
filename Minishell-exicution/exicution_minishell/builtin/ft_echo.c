/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:51:22 by maskour           #+#    #+#             */
/*   Updated: 2025/05/05 17:45:23 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int nb_args(char **arg)
{
    int i = 0;
    while (arg[i])
        i++;
    return (i);
}
void ft_echo(t_cmd **cmd)
{
    int i = 1;
    int n_nb = 0;
    t_cmd *current_cmd = *cmd;
    if (nb_args(current_cmd->cmd) > 1)
    {
        while (current_cmd->cmd[i] && ft_strcmp(current_cmd->cmd[i], "-n") == 0)
        {
            n_nb = 1;
            i++;
        }
        ft_putstr_fd_up(current_cmd->cmd[i],1);
        while (current_cmd->cmd[i])
        {
            if(current_cmd->cmd[i+1])
                printf(" ");
            i++;
        }
    }
    if (n_nb == 0)
        printf("\n");
}
