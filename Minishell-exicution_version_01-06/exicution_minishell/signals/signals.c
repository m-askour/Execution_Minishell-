/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 20:33:01 by maskour           #+#    #+#             */
/*   Updated: 2025/07/03 15:38:47 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


void handler_sig(int signal)
{

    if (signal == SIGINT)
	{
    	write(1,"\n",1);
  		rl_on_new_line();
  		rl_replace_line("", 0);
  		rl_redisplay();
		// if (g_shell_ctx)
        //     g_shell_ctx->exit_status = 1;
	}
	else if (signal == SIGQUIT)//THIS TO KILL THE PROCESSE OR SHUTDOWN REQUEST
    	rl_redisplay();	
}
