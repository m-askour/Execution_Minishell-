/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:51:33 by maskour           #+#    #+#             */
/*   Updated: 2025/07/09 16:38:24 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_pwd(t_shell *shell_ctx, t_env *env_list)
{
	char cwd[PATH_MAX];
	t_env *env;

	env = env_list;
	if (getcwd(cwd, sizeof(cwd)))
	{
			printf("%s\n",cwd);
			shell_ctx->exit_status = 0;
	}
	else
	{
	while (env)
	{
		if (strncmp(env->data_env, "PWD=", 4) == 0)
		{
			printf("%s\n", env->data_env + 4);
			shell_ctx->exit_status = 0;
			return ;
		}
		env = env->next;
	}
	shell_ctx->exit_status = 1;
	perror("PWD not found in environment\n");  
	}
}
