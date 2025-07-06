/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:51:22 by maskour           #+#    #+#             */
/*   Updated: 2025/06/28 21:13:09 by maskour          ###   ########.fr       */
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
    if (cmd[0] != '-')
        return 0;
    i = 1;
    if (!cmd[i]) // if only '-', not a valid flag
        return 0;
    while (cmd[i])
    {
        if (cmd[i] != 'n')
            return 0;
        i++;
    }
    return 1;
}
void ft_echo(t_cmd **cmd, t_shell *shell_ctx)
{
    int i = 1;
    int n_nb = 0;
    t_cmd *current_cmd = *cmd;

    if (nb_args(current_cmd->cmd) > 1)
    {
        // Handle -n options
        while (current_cmd->cmd[i] && check_new_line(current_cmd->cmd[i]))
        {
            n_nb = 1;
            i++;
        }
        // Print arguments
        while (current_cmd->cmd[i])
        {
            write(STDOUT_FILENO, current_cmd->cmd[i], strlen(current_cmd->cmd[i]));
            if (current_cmd->cmd[i + 1])
                write(STDOUT_FILENO, " ", 1);
            i++;
        }
    }

    if (n_nb == 0)
        write(STDOUT_FILENO, "\n", 1);

    shell_ctx->exit_status = 0;
}
