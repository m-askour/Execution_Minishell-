/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execut_multiple_pipe.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:34:35 by maskour           #+#    #+#             */
/*   Updated: 2025/07/05 20:49:35 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void chiled_process(int i,int cmd_count, int prev_pipe, int pipes[2], t_cmd **cmds, t_shell *shell_ctx, t_env *env)
	{
			signal(SIGQUIT, handler_sig);
			signal(SIGINT, handler_sig);
			if (i > 0)
			{
				if (dup2(prev_pipe, STDIN_FILENO) == -1)
				{
					perror("minishell: dup2");
					exit(1);
				}
				close(prev_pipe);
			}
			if (i < cmd_count - 1)
			{
				close(pipes[0]);
				if (dup2(pipes[1], STDOUT_FILENO) == -1)
				{
					perror("minishell: dup2");
					exit(1);
				}
				close(pipes[1]);
			}
			if (i > 0 && prev_pipe != -1)
				close(prev_pipe);
			if (i < cmd_count - 1)
				close(pipes[0]);
			if (!cmds[i]->cmd || !cmds[i]->cmd[0])
			{
				ft_putstr_fd_up("minishell: empty command\n", 2);
				exit(127);
			}
			if (redirections(cmds[i]))
				exit(1);
			if (is_builtin(cmds[i]->cmd[0]))
				exit(shell_ctx->exit_status);
			char *path = find_path(cmds[i]->cmd[0], env);
			if (!path)
			{
				handle_cmd_errors(path, ": command not found\n");
				exit(127);
			}
			if (execve(path, cmds[i]->cmd, env) == -1)
			{
				handle_cmd_errors(path, "execve filed\n");
				free(path);
				exit(126);
			}
			free(path);
			exit(126);
}

void wait_status(t_shell *shell_ctx, pid_t last_pid)
{
	int wstatus = 0;
	pid_t wpid;
	while ((wpid = wait(&wstatus)) > 0)
	{
		if (wpid == last_pid)
		{
			if (WIFEXITED(wstatus))
				shell_ctx->exit_status = WEXITSTATUS(wstatus);
			else if (WIFSIGNALED(wstatus))
				{
                    shell_ctx->exit_status = 128 + WTERMSIG(wstatus);
			        if (WTERMSIG(wstatus) == SIGINT)
				        write (1,"\n",1);
                }
			else
				shell_ctx->exit_status = 1;
		}
	}
}

void execute_pipeline(t_cmd **cmds, int cmd_count, char **env,t_shell *shell_ctx)
{
	if (!cmds || cmd_count <= 0)
		return;
	int pipes[2];
	pid_t pid;
	int prev_pipe = -1;
	pid_t last_pid = -1;
	int i = 0;
	signal(SIGINT, SIG_IGN);
	while (i < cmd_count && cmds[i] != NULL)
	{
		if (i < cmd_count - 1 && pipe(pipes) == -1)
		{
			perror("minishell: pipe");
			exit(1);
		}
		pid = fork();
		if (pid == 0)
			chiled_process();
		else if (pid > 0)
			parent_process();
		else
		{
			perror("minishell: fork");
			exit(1);
		}
		i++;
	}
	if (prev_pipe != -1)
		close(prev_pipe);
	wait_status(shell_ctx, last_pid);
	restore_sigint();
}
