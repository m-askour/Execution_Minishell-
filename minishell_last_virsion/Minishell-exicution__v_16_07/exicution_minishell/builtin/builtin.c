/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 18:48:55 by maskour           #+#    #+#             */
/*   Updated: 2025/07/16 21:55:52 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_builtin(char *command)
{
	if (!command)
		return (0);
	return (ft_strcmp(command, "cd") == 0 || \
		ft_strcmp(command, "echo") == 0 || \
		ft_strcmp(command, "exit") == 0 || \
		ft_strcmp(command, "export") == 0 || \
		ft_strcmp(command, "unset") == 0 || \
		ft_strcmp(command, "env") == 0 || \
		ft_strcmp(command, "pwd") == 0);
}

static int	handle_redirection(t_cmd **cmd, int stdin_copy,
					int stdout_copy, t_shell *shell)
{
	if (redirections(*cmd, 0) != 0)
	{
		dup2(stdin_copy, STDIN_FILENO);
		dup2(stdout_copy, STDOUT_FILENO);
		close(stdin_copy);
		close(stdout_copy);
		shell->exit_status = 1;
		return (0);
	}
	return (1);
}

static void	dup_files(int stdin_c, int stdout_c)
{
	dup2(stdin_c, STDIN_FILENO);
	dup2(stdout_c, STDOUT_FILENO);
}

static void	close_all_file(int stdin_c, int stdout_c)
{
	close(stdin_c);
	close(stdout_c);
}

t_env	*execut_bultin(t_cmd **cmd, t_env *env_list, t_shell *shell, int i)
{
	int	stdin_c;
	int	stdout_c;

	stdin_c = dup(STDIN_FILENO);
	stdout_c = dup(STDOUT_FILENO);
	if (!handle_redirection(cmd, stdin_c, stdout_c, shell))
		return (env_list);
	if (ft_strcmp(cmd[0]->cmd[0], "cd") == 0)
		env_list = ft_cd(cmd, env_list, shell);
	else if (ft_strcmp(cmd[0]->cmd[0], "echo") == 0)
		ft_echo(cmd, shell);
	else if (ft_strcmp(cmd[0]->cmd[0], "exit") == 0)
		ft_exit(cmd, shell, i);
	else if (ft_strcmp(cmd[0]->cmd[0], "export") == 0)
		ft_export(cmd, &env_list, shell);
	else if (ft_strcmp(cmd[0]->cmd[0], "unset") == 0)
		env_list = ft_unset(cmd, env_list, shell);
	else if (ft_strcmp(cmd[0]->cmd[0], "env") == 0)
		ft_env(env_list, shell);
	else if (ft_strcmp(cmd[0]->cmd[0], "pwd") == 0)
		ft_pwd(shell, env_list);
	if (i)
		dup_files(stdin_c, stdout_c);
	close_all_file(stdin_c, stdout_c);
	return (env_list);
}
