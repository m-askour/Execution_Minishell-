/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:51:33 by maskour           #+#    #+#             */
/*   Updated: 2025/05/02 21:25:23 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void ft_pwd()
{
    char cwd[PATH_MAX];//the path max is in inlude <limits.h>

    if (getcwd(cwd, sizeof(cwd)))// getcwd in include<unistd.h>
    {
            printf("%s\n",cwd);    
    }
    else
        perror("PWD not found in environment\n");
}
