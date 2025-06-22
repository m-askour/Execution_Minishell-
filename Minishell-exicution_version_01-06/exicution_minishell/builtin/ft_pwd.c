/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:51:33 by maskour           #+#    #+#             */
/*   Updated: 2025/06/19 15:11:03 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void ft_pwd(t_shell *shell_ctx)
{
    char cwd[PATH_MAX];//the path max is in inlude <limits.h>
    if (getcwd(cwd, sizeof(cwd)))// getcwd in include<unistd.h>
    {
            printf("%s\n",cwd);
            shell_ctx->exit_status = 0;
    }
    else
    {
        shell_ctx->exit_status = 1;
        perror("PWD not found in environment\n");
    }
}
