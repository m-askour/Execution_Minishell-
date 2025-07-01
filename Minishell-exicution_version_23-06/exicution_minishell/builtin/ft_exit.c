/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 19:01:03 by maskour           #+#    #+#             */
/*   Updated: 2025/06/25 23:19:32 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void exit_number_error(char *arg)
{
	ft_putstr_fd_up("minishell:",2);
	ft_putstr_fd_up(arg, 2);
	ft_putstr_fd_up(": numeric argument required\n",2);
	exit(127);
}

void ft_exit(t_cmd **cmd, t_shell *shell, int j)
{
	int		status;
	int		i;
	t_cmd	*current_cmd;

	status = shell->exit_status;
	i = 0;
	current_cmd = *cmd;
	if (j == 1)
		printf("exit\n");
	if (!current_cmd->cmd[1]){ exit(shell->exit_status);}
	if (current_cmd->cmd[1][i] == '+' || current_cmd->cmd[1][i] == '-')
		i++;
	if (!current_cmd->cmd[1][i])
		exit_number_error(current_cmd->cmd[i]);
	while(current_cmd->cmd[1][i])
	{  
		if (!ft_isdigit(current_cmd->cmd[1][i]))
			exit_number_error(current_cmd->cmd[1]);
		i++;
	}
	if (current_cmd->cmd[2] != NULL)
		return (ft_putstr_fd_up("minishell: exit: too many arguments\n",2));
	status = ft_atoi(current_cmd->cmd[1]) % 256;
	if (status < 0) {status +=256;}
	exit(status);
}
