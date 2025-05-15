/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:51:22 by maskour           #+#    #+#             */
/*   Updated: 2025/05/14 20:29:02 by maskour          ###   ########.fr       */
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
static int check_new_line(char *cmd)
{
    int i = 0;
    
    if (cmd[0] == '-')
            i++;
    while (cmd[i])
    {
        // printf("%c\n",cmd[i]);
        if (cmd[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}
void ft_echo(t_cmd **cmd)
{
    int i = 1;
    int n_nb = 0;
    t_cmd *current_cmd = *cmd;
    if (nb_args(current_cmd->cmd) > 1)
    {

        while (current_cmd->cmd[i] && check_new_line(current_cmd->cmd[i]))
        {
            n_nb = 1;
            i++;
        }
        while (current_cmd->cmd[i])
        {
            printf("%s",current_cmd->cmd[i]);
            if(current_cmd->cmd[i+1])
                printf(" ");
            i++;
        }
        
    }
    if (n_nb == 0)
        printf("\n");
}
