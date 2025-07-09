/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 18:49:15 by maskour           #+#    #+#             */
/*   Updated: 2025/07/09 16:04:38 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	add_oldpwd(t_env *data_env)
{
	t_env	*curreent;
	t_env	*last;
	char	*new_path;
	t_env *new_node;
	
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
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (free(new_path));
	new_node->data_env = new_path;
	new_node->next = NULL;
	if (last)
		last->next = new_node;
}

static void	update_env_var(t_env *data_env, char *key, char *dest)
{
	t_env *current;
	char *new_path;

	current = data_env;
	while (current)
	{
		if (ft_strncmp(current->data_env, key, ft_strlen(key)) == 0)
		{
			new_path = ft_strjoin(key, dest);
			if (!new_path)
				return;
			free(current->data_env);
			current->data_env = new_path;
			return;
		}
		current = current->next;
	}
}
static char *validate_path(t_cmd *cmd_path, t_env *data_env, t_shell *shell_ctx)
{
	char *path;

	if (!cmd_path->cmd[1] || !ft_strcmp(cmd_path->cmd[1], "~"))
	{
		path = search_env(data_env, "HOME");
		if (!path)
		{
			ft_putstr_fd_up("minishell: cd: HOME not set\n", 2);
			shell_ctx->exit_status = 1;
			return (NULL);
		}
	}
	else if (!ft_strcmp(cmd_path->cmd[1], "-"))
	{
		path = search_env(data_env, "OLDPWD");
		if (!path)
		{
			ft_putstr_fd_up("minishell: cd: OLDPWD not set\n", 2);
			shell_ctx->exit_status = 1;
			return 	(NULL);
		}
	}
	else
		path = cmd_path->cmd[1];
	return (path);
}

static void check_getcwd(char *oldpwd_update, t_cmd *cmd_path, t_shell *shell_ctx)
{
	if (!getcwd(oldpwd_update, PATH_MAX))
	{
		if (ft_strncmp(cmd_path->cmd[1],".",2) == 0)
		{
			
			if (access(".", F_OK | X_OK) == 0)
			{
				perror("minishell: cd: error retrieving current directory: getcwd: cannot access parent directories");
			}
		}
		shell_ctx->exit_status = 1;
	}
}
static int access_path(char *path, t_shell *shell_ctx)
{
	if (access(path, F_OK) != 0)
	{
		ft_putstr_fd_up("minishell: cd: ", 2);
		ft_putstr_fd_up(path, 2);
		ft_putstr_fd_up(": No such file or directory\n", 2);
		shell_ctx->exit_status = 1;
		return 0;
	}
	if (access(path,X_OK) != 0)
	{
		ft_putstr_fd_up("minishell: cd: ", 2);
		ft_putstr_fd_up(path, 2);
		ft_putstr_fd_up(": Permission denied\n", 2);
		shell_ctx->exit_status = 1;
		return 0;
	}
	return (1);
}
static void update(t_env *data_env, char *oldpwd_update, char *pwd_update)
{
	add_oldpwd(data_env);
	update_env_var(data_env, "OLDPWD=", oldpwd_update);
	update_env_var(data_env, "PWD=", pwd_update);
}
static int check_cmd(t_cmd *cmd_path, t_shell *shell_ctx)
{
	if (!cmd_path || !cmd_path->cmd || !cmd_path->cmd[0])
	{
		shell_ctx->exit_status = 1;
		return (0);
	}
	return (1);
}	
static int checkaccec_cmd(t_shell *shell_ctx)
{
	if (access(".", F_OK | X_OK) != 0)
	{
		perror("minishell: cd: error retrieving current directory: getcwd: cannot access parent directories");
		shell_ctx->exit_status = 1;
		return (0);
	}
	return (1);
}
static int check_path_2(char * path, char *pwd_update, t_shell *shell_ctx)
{
	if (chdir(path) != 0)
	{
		ft_putstr_fd_up("minishell: cd: ", 2);
		perror(path);
		shell_ctx->exit_status = 1;
		return (0);
	}
	if (!getcwd(pwd_update, PATH_MAX))
	{
		shell_ctx->exit_status = 1;
		return (0);
	}
	return (1);
}
static void check_update_pwd(t_cmd *cmd_path, char *pwd_update)
{
	if (!ft_strcmp(cmd_path->cmd[1], "-"))
	{
		ft_putstr_fd_up(pwd_update, 1);
		ft_putstr_fd_up("\n", 1);
	}
}
t_env	*ft_cd(t_cmd **cmd, t_env *data_env, t_shell *shell_ctx)
{
	char pwd_update[PATH_MAX];
	char oldpwd_update[PATH_MAX];
	char *path;
	t_cmd *cmd_path;

	cmd_path = *cmd;
	if (!check_cmd(cmd_path, shell_ctx))
		return (data_env);
	if (!checkaccec_cmd(shell_ctx))
		return (data_env);
	check_getcwd(oldpwd_update, cmd_path, shell_ctx);
	path = validate_path(cmd_path, data_env, shell_ctx);
	if (!path)
		return (data_env);
	if (!access_path(path, shell_ctx))
		return(data_env);
	if (!check_path_2(path, pwd_update, shell_ctx))
		return (data_env);
	update(data_env, oldpwd_update, pwd_update);
	check_update_pwd(cmd_path, pwd_update);
	shell_ctx->exit_status = 0;
	return data_env;
}




//this not norm
// t_env *ft_cd(t_cmd **cmd, t_env *data_env, t_shell *shell_ctx)
// {
// 	char pwd_update[PATH_MAX];
// 	char oldpwd_update[PATH_MAX];
// 	char *path = NULL;
// 	t_cmd *cmd_path = *cmd;

// 	if (!cmd_path || !cmd_path->cmd || !cmd_path->cmd[0])
// 	{
// 		shell_ctx->exit_status = 1;
// 		return data_env;
// 	}
// 	if (ft_strcmp(cmd_path->cmd[0], "cd") != 0)
// 		return data_env;
// 	if (access(".", F_OK | X_OK) != 0)
// 	{
// 		perror("minishell: cd: error retrieving current directory: getcwd: cannot access parent directories");
// 		shell_ctx->exit_status = 1;
// 		return data_env;
// 	}
// 	if (!cmd_path->cmd[1])
// 	{
// 		shell_ctx->exit_status = 0;
// 		return data_env;
// 	}
// 	if (!getcwd(oldpwd_update, PATH_MAX))
// 	{
// 		if (ft_strncmp(cmd_path->cmd[1],".",2) == 0)
// 		{
			
// 			if (access(".", F_OK | X_OK) == 0)
// 			{
// 				perror("minishell: cd: error retrieving current directory: getcwd: cannot access parent directories");
// 			}
// 		}
// 		shell_ctx->exit_status = 1;
// 	}
// 	if (!ft_strcmp(cmd_path->cmd[1], "~") || !cmd_path->cmd[1])
// 	{
// 		path = search_env(data_env, "HOME");
// 		if (!path)
// 		{
// 			ft_putstr_fd_up("minishell: cd: HOME not set\n", 2);
// 			shell_ctx->exit_status = 1;
// 			return data_env;
// 		}
// 	}
// 	else if (!ft_strcmp(cmd_path->cmd[1], "-"))
// 	{
// 		path = search_env(data_env, "OLDPWD");
// 		if (!path)
// 		{
// 			ft_putstr_fd_up("minishell: cd: OLDPWD not set\n", 2);
// 			shell_ctx->exit_status = 1;
// 			return data_env;
// 		}
// 	}
// 	else
// 		path = cmd_path->cmd[1];
// 	if (access(path, F_OK) != 0)
// 	{
// 		ft_putstr_fd_up("minishell: cd2: ", 2);
// 		ft_putstr_fd_up(path, 2);
// 		ft_putstr_fd_up(": No such file or directory\n", 2);
// 		shell_ctx->exit_status = 1;
// 		return data_env;
// 	}
// 	if (access(path,X_OK) != 0)
// 	{
// 		ft_putstr_fd_up("minishell: cd1: ", 2);
// 		ft_putstr_fd_up(path, 2);
// 		ft_putstr_fd_up(": Permission denied\n", 2);
// 		shell_ctx->exit_status = 1;
// 		return data_env;
// 	}
// 	if (chdir(path) != 0)
// 	{
// 		ft_putstr_fd_up("minishell: cd: ", 2);
// 		perror(path);
// 		shell_ctx->exit_status = 1;
// 		return data_env;
// 	}
// 	if (!getcwd(pwd_update, PATH_MAX))
// 	{
// 		shell_ctx->exit_status = 1;
// 		return data_env;
// 	}
// 	add_oldpwd(data_env);
// 	update_env_var(data_env, "OLDPWD=", oldpwd_update);
// 	update_env_var(data_env, "PWD=", pwd_update);
// 	if (!ft_strcmp(cmd_path->cmd[1], "-"))
// 	{
// 		ft_putstr_fd_up(pwd_update, 1);
// 		ft_putstr_fd_up("\n", 1);
// 	}
// 	shell_ctx->exit_status = 0;
// 	return data_env;
// }