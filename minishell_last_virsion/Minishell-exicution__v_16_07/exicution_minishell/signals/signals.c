/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 20:33:01 by maskour           #+#    #+#             */
/*   Updated: 2025/07/16 22:28:33 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handler_sig(int signal)
{

    if (signal == SIGINT)
	{
    	write(1,"\n",1);
  		rl_on_new_line();
  		rl_replace_line("", 0);
  		rl_redisplay();
	}
	else if (signal == SIGQUIT)
    	rl_redisplay();	
}
