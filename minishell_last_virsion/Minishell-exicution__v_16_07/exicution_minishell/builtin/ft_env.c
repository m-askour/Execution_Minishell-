/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:51:17 by maskour           #+#    #+#             */
/*   Updated: 2025/07/16 22:12:56 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_env(t_env *env_list, t_shell *shell_ctx)
{
	t_env	*env;

	env = env_list;
	while (env != NULL)
	{
		if (ft_strchr(env->data_env, '='))
			printf("%s\n", env->data_env);
		env = env->next;
	}
	shell_ctx->exit_status = 0;
}
