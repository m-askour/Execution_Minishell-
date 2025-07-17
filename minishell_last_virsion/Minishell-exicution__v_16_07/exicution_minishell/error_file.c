/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:23:28 by maskour           #+#    #+#             */
/*   Updated: 2025/07/16 22:30:34 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_cmd_errors(char *cmd_path, char *message)
{
	if(cmd_path)
	{
		ft_putstr_fd_up("minishell:", 2);
		ft_putstr_fd_up(cmd_path, 2);
		ft_putstr_fd_up(message, 2);
	}
}
