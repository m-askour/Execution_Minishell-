/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 18:49:15 by maskour           #+#    #+#             */
/*   Updated: 2025/06/20 20:39:02 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	add_oldpwd (t_env *data_env)
{
	t_env	*curreent;
	t_env	*last;
	char	*new_path;

	curreent = data_env;
	last = NULL;
	while (curreent)
	{
		if (ft_strstr(curreent->data_env, "OLDPWD="))
				return ;
			last = curreent;
			curreent = curreent->next;
	}
	new_path = ft_strjoin("OLDPWD", "=");
	if (!new_path)
		return ;
	t_env *new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		free(new_path);
		return ;
	}
	new_node->data_env = new_path;
	new_node->next = NULL;
	if (last)
		last->next = new_node;
}

static void update_env_var(t_env *data_env, char *key, char *dest)
{
	t_env	*current;
	char	*new_path;

	current = data_env;
	while(current)
	{
		if (ft_strstr(current->data_env,key) != NULL)
		{
			new_path = ft_strjoin(key, dest);
			if (!new_path)
				 return ;
			free(current->data_env);
			current->data_env = new_path;
			return ;
		}
		current = current->next;
	}

}

t_env *ft_cd(t_cmd **cmd, t_env *data_env, t_shell *shell_ctx)
{
	char pwd_update[PATH_MAX];
	char oldpwd_update[PATH_MAX];
	char *path = NULL;
	t_cmd *cmd_path;

	cmd_path = *cmd;
	if (!cmd_path->cmd[1])
	{
		shell_ctx->exit_status = 0;
		return (data_env);
	}
	if (!getcwd(oldpwd_update, PATH_MAX))
	{
		perror("minishell: cd");
		shell_ctx->exit_status = 1;
		return (data_env);
	}
	if (cmd_path->cmd[1] && !ft_strcmp(cmd_path->cmd[1], "~"))
		{
			path = search_env(data_env,"HOME");
			printf("%s\n", path);
			if (!path)
			{
				ft_putstr_fd_up("minishell: cd: HOME not set\n",2);
				shell_ctx->exit_status = 1;
				return (data_env);
			}
		}
	else if (!ft_strcmp(cmd_path->cmd[1], "-") && cmd_path->cmd[1])
	{  
		path = search_env(data_env, "OLDPWD");
		if (!path)
		{
			ft_putstr_fd_up("minishell: cd: OLDPWD not set\n",2);
			shell_ctx->exit_status = 1;
			return (data_env);
		}
	}
	else
		path = cmd_path->cmd[1];
	if (chdir(path) != 0)
	{
		ft_putstr_fd_up("minishell: cd: ", 2);
		perror(path);
		shell_ctx->exit_status = 1;
		return (data_env);
	}
	if (!getcwd(pwd_update, PATH_MAX))
	{
		perror("minishell: cd: getcwd error");
		return (data_env);
	}
	add_oldpwd(data_env);
	update_env_var(data_env, "OLDPWD=", oldpwd_update);
	update_env_var(data_env, "PWD=", pwd_update);
	if (cmd_path->cmd[1] && !ft_strcmp(cmd_path->cmd[1], "-"))
	{
		ft_putstr_fd_up(pwd_update, 1); 
		ft_putstr_fd_up("\n", 1);
	}
	shell_ctx->exit_status = 0;
	return (data_env);
}
