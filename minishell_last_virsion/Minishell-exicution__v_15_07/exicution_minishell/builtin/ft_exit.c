/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 19:01:03 by maskour           #+#    #+#             */
/*   Updated: 2025/07/15 20:55:50 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	exit_number_error(char *arg)
{
	ft_putstr_fd_up("minishell:",2);
	ft_putstr_fd_up(arg, 2);
	ft_putstr_fd_up(": numeric argument required\n",2);
	exit(255);
}

static int	ft_swap_sign(const char *str, int *i)
{
	int	sign;

	sign = 1;
	while ((str[*i] >= 9 && str[*i] <= 13) || str[*i] == 32)
		(*i)++;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			sign = sign * -1;
		(*i)++;
	}
	return (sign);
}

static long	ft_atol(const char *str)
{
	int			i;
	long long	nb;
	int			sign;
	long long	result;

	i = 0;
	nb = 0;
	result = 0;
	sign = ft_swap_sign(str, &i);
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = str[i] + (nb * 10) - '0';
		if (nb / 10 < result)
		{
			if (sign == 1)
				return (-1);
			else
				return (0);
		}
		result = nb;
		i++;
	}
	return (nb * sign);
}

static int	is_valid_exit(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (!arg[i])
		return (0);
	while(arg[i])
	{  
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(t_cmd **cmd, t_shell *shell, int j)
{
	int		status;
	char	*arg;
	t_cmd	*current_cmd;

	status = shell->exit_status;
	current_cmd = *cmd;
	arg = current_cmd->cmd[1];
	if (!arg)
		exit(shell->exit_status);
	if (current_cmd->cmd[2] != NULL)
	{
		ft_putstr_fd_up("minishell: exit: too many arguments\n",2);
		shell->exit_status = 1;
		return ;
	}
	if (j == 1)
		printf("exit\n");
	if (!is_valid_exit(arg) || ft_atol(arg) == -1 || ft_atoi(arg) == 0)
		exit_number_error(arg);
	status = ft_atoi(arg) % 256;
	if (status < 0) 
		status +=256;
	exit(status);
}
