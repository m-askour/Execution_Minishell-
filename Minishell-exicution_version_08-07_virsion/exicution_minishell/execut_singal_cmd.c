/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execut_singal_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:42:06 by maskour           #+#    #+#             */
/*   Updated: 2025/07/13 12:47:38 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int process_heredocs(t_cmd *cmd, char **env, t_shell *shell, int *last_heredoc_index, int original_stdin, int original_stdout)
{
	int i = -1;
	while (++i < cmd->file_count)
	{
		if (cmd->files[i].type == TOKEN_HEREDOC)
		{
			int hd_status = function_herdoc(&cmd->files[i], env, shell);
			if (hd_status == 130 || hd_status != 0)
			{
				cleanup_stdio(original_stdin, original_stdout);
				exit(hd_status);
			}
			*last_heredoc_index = i;
		}
	}
	return 0;
}

static void handle_empty_cmd(t_cmd *cmd)
{
	if (!cmd || !cmd->cmd)
	{
		ft_putstr_fd_up("minishell:", 2);
		ft_putstr_fd_up(" command not found\n", 2);
		exit(127);
	}
	if (!cmd->cmd[0])
	{
		handle_cmd_errors(NULL);
		exit(0);
	}
}

static char *resolve_cmd_path(t_cmd *cmd, char **env)
{
	char *cmd_path = find_path(cmd->cmd[0], env);
	if (!cmd_path)
	{
		if (access(cmd->cmd[0], F_OK) == 0)
		{
			ft_putstr_fd_up("minishell: ", 2);
			ft_putstr_fd_up(cmd->cmd[0], 2);
			ft_putstr_fd_up(": Permission denied\n", 2);
			exit(126);
		}
		ft_putstr_fd_up("minishell:", 2);
		ft_putstr_fd_up(cmd->cmd[0], 2);
		ft_putstr_fd_up(": command not found\n", 2);
		exit(127);
	}
	return cmd_path;
}

static void execute_command(char *cmd_path, t_cmd *cmd, char **env)
{
	if (execve(cmd_path, cmd->cmd, env) == -1)
	{
		if (cmd->cmd[0][0] == '\0')
		{
			ft_putstr_fd_up("minishell:", 2);
			ft_putstr_fd_up(" command not found\n", 2);
			exit(127);
		}
		handle_cmd_errors(cmd_path);
		free(cmd_path);
		exit(126);
	}
}

static int cmd_process(t_cmd *cmd, char **env, t_shell *shell)
{
	char *cmd_path;
	int original_stdin = dup(STDIN_FILENO);
	int original_stdout = dup(STDOUT_FILENO);
	int last_heredoc_index = -1;

	process_heredocs(cmd, env, shell, &last_heredoc_index, original_stdin, original_stdout);
	handle_empty_cmd(cmd);

	if (redirections(cmd, last_heredoc_index) != 0)
		exit(0);

	cmd_path = resolve_cmd_path(cmd, env);
	execute_command(cmd_path, cmd, env);

	exit(0);
}

static void setup_terminal_and_signals(struct termios *original_termios)
{
	if (tcgetattr(STDIN_FILENO, original_termios) == -1)
		perror("tcgetattr failed");
	ignore_sigint();
}

static void restore_terminal_and_signals(struct termios *original_termios)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, original_termios) == -1)
		perror("tcsetattr failed");
	restore_sigint();
}

static void handle_child_exit_status(int status, t_shell *shell_ctx)
{
	if (WIFEXITED(status))
	{
		int code = WEXITSTATUS(status);
		if (code == 130)
		{
			shell_ctx->exit_status = 1;
			write(1, "\n", 1);
		}
		else if (code == 2)
			shell_ctx->exit_status = 0;
		else
			shell_ctx->exit_status = code;
	}
	else if (WIFSIGNALED(status))
	{
		shell_ctx->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit\n", 5);
	}
	else
		shell_ctx->exit_status = 0;
}

static void execute_single_command(t_cmd **cmd, char **envp, t_shell *shell_ctx)
{
	pid_t id;
	int status;
	struct termios original_termios;

	setup_terminal_and_signals(&original_termios);

	id = fork();
	if (id == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		cmd_process(*cmd, envp, shell_ctx);
		exit(0);
	}
	else if (id > 0)
	{
		waitpid(id, &status, 0);
		restore_terminal_and_signals(&original_termios);
		handle_child_exit_status(status, shell_ctx);
	}
	else
	{
		perror("fork failed");
		restore_terminal_and_signals(&original_termios);
	}
}
