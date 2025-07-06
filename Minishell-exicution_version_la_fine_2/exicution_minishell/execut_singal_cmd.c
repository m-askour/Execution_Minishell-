/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execut_singal_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:38:53 by maskour           #+#    #+#             */
/*   Updated: 2025/06/24 18:39:14 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void cmd_process(t_cmd *cmd, char **env)
{
	char *cmd_path;
	if (!cmd) { handle_cmd_errors(NULL, NULL); exit(1); }
	if (redirections(cmd)) exit(1);
	if (!cmd->cmd || !cmd->cmd[0]) { handle_cmd_errors(NULL, NULL); exit(1); }
	cmd_path = find_path(cmd->cmd[0], env);
	if (!cmd_path)
	{
		handle_cmd_errors(cmd->cmd[0], ": command not found\n");
		exit(127);
	}
	if (execve(cmd_path, cmd->cmd, env) == -1)
	{
		handle_cmd_errors(cmd_path, "execve failed\n");
		free(cmd_path);
		exit(126);
	}
	exit(0);
}

static void parent_process(pid_t id, int *status, t_shell *shell_ctx)
{
		waitpid(id, status, 0);
		restore_sigint();
		if (WIFEXITED(*status))
			shell_ctx->exit_status = WEXITSTATUS(*status);
		else if (WIFSIGNALED(*status))
		{
			shell_ctx->exit_status = 128 + WTERMSIG(*status);
			if (WTERMSIG(*status) == SIGINT)
				write (1,"\n",1);
		}
		else
			shell_ctx->exit_status = 1;
}

void execute_single_command(t_cmd **cmd, char **envp, t_shell *shell_ctx)
{
	pid_t id;
	int status;
	
	ignore_sigint();
	id = fork();
	if (id == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		cmd_process(*cmd, envp);
		shell_ctx->exit_status = 0;
	}
	else if (id > 0)
		parent_process(id, &status, shell_ctx);
	else
	{
		perror("fork failed");
		restore_sigint();
	}
}