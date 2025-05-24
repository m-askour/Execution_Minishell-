/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:51:17 by maskour           #+#    #+#             */
/*   Updated: 2025/05/02 21:25:14 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
void ft_env(t_env *env_list)
{
    t_env *env = env_list;
    while (env != NULL)
    {
        printf("%s\n",env->data_env);
        env = env->next;
    }
}