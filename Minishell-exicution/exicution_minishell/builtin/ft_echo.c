/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:51:22 by maskour           #+#    #+#             */
/*   Updated: 2025/05/03 09:38:31 by maskour          ###   ########.fr       */
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
void ft_echo(char **cmd)
{
    int i = 1;
    int n_nb = 0;
    if (nb_args(cmd) > 1)
    {
        while (cmd[i] && ft_strcmp(cmd[i], "-n") == 0)
        {
            n_nb = 1;
            i++;
        }
        ft_putstr_fd_up(cmd[i],1);
        while (cmd[i])
        {
            if(cmd[i+1] && cmd[i][0] != '\0')
                printf(" ");
            i++;
        }
    }
    if (n_nb == 0)
        printf("\n");
}
