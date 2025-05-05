/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 19:01:03 by maskour           #+#    #+#             */
/*   Updated: 2025/05/03 09:38:36 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void ft_exit(t_cmd **cmd)
{
    int status = 0;
    if (!cmd[0]->cmd[1])
        exit(0);
    else if (!ft_isalnum(cmd[0]->cmd[1]))
    {
        ft_putstr_fd_up("numeric argument required",2);
        exit(2);
    }
    else if (cmd[0]->cmd[2] != NULL)
    {
        perror("too many arguments");
        return ;
    }
    else
    {
        status = ft_atoi(cmd[0]->cmd[1]) % 256;
        if (status < 0)
            status +=256;
        exit(status);
    }
    
}
