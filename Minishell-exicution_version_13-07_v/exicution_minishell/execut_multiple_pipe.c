/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execut_multiple_pipe.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:47:51 by maskour           #+#    #+#             */
/*   Updated: 2025/07/13 13:47:29 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



static void handle_cmd_errors(char *cmd_path)
{
    if(cmd_path)
	{
	    ft_putstr_fd_up("minishell: 1", 2);
        ft_putstr_fd_up(cmd_path, 2);
        ft_putstr_fd_up(": execution failed\n", 2);
	}
}

static void free_env(char **env)
{
    if (!env)
        return ;
    int i = 0;
    while(env[i])
    {
        free(env[i]);
        i++;
    }
    free(env);
}
static void ignore_sigint(void)
{
    signal(SIGINT, SIG_IGN);
}

static void restore_sigint(void)
{
    signal(SIGINT, handler_sig);
}
static int is_spaces(char *s)
{
    if (!s)
        return (1);
    while (*s)
    {
        if (*s != ' ')
            return (0);
        s++;
    }
    return (1);
}
static void cleanup_heredoc_files(t_cmd **cmds, int cmd_count)
{
    int j;
    j = -1;
    int k;
    while  (++j < cmd_count)
    {
        t_cmd *cmd = cmds[j];
        k = -1;
        while (++k < cmd->file_count)
        {
            if (cmd->files[k].type == TOKEN_HEREDOC)
                unlink(cmd->files[k].name);
        }
    }
}
static int find_last_heredoc_index(t_cmd *cmd)
{
    int i;
    int last = -1;
    i = -1;
    while (++i < cmd->file_count)
    {
        if (cmd->files[i].type == TOKEN_HEREDOC)
            last = i;
    }
    return last;
}
static int process_all_heredocs(t_cmd **cmds, int cmd_count, char **env, t_shell *shell_ctx)
{
    int h = -1, f;
    while (++h < cmd_count)
    {
        f = -1;
        while (++f < cmds[h]->file_count)
        {
            if (cmds[h]->files[f].type == TOKEN_HEREDOC)
            {
                int hd_status = function_herdoc(&cmds[h]->files[f], env, shell_ctx);
                if (hd_status == 130 || hd_status != 0)
                {
                    shell_ctx->exit_status = 1;
                    cleanup_heredoc_files(cmds, cmd_count);
                    signal(SIGINT, SIG_IGN);
                    return 0;
                }
            }
            signal(SIGINT, SIG_IGN);
        }
    }
    return (1);
}

static void setup_pipes_in_child(int i, int cmd_count, int prev_pipe, int pipes[2])
{
    if (i > 0)
    {
        dup2(prev_pipe, STDIN_FILENO);
        close(prev_pipe);
    }
    if (i < cmd_count - 1)
    {
        close(pipes[0]);
        dup2(pipes[1], STDOUT_FILENO);
        close(pipes[1]);
    }
}

static void handle_cmd_not_found(char *cmd, int i)
{
    if (access(cmd, F_OK) == 0)
    {
        ft_putstr_fd_up("minishell: ", 2);
        ft_putstr_fd_up(cmd, 2);
        ft_putstr_fd_up(": Permission denied\n", 2);
        exit(126);
    }

    if (i == 0)
    {
        if (ft_strstr(cmd, "./") != NULL)
        {
            ft_putstr_fd_up("minishell:", 2);
            ft_putstr_fd_up(cmd, 2);
            ft_putstr_fd_up(":No such file or directory\n", 2);
        }
        else
        {
            ft_putstr_fd_up("minishell: ", 2);
            ft_putstr_fd_up(cmd, 2);
            ft_putstr_fd_up(": command not found\n", 2);
        }

    }
    else
        ft_putstr_fd_up("0", 2);
    exit(127);
}

static void execute_external_command(char *cmd, char **argv, char **env, int i)
{
    char *path = find_path(cmd, env);
    if (!path)
        handle_cmd_not_found(cmd, i);

    if (execve(path, argv, env) ==-1)
    {
        if (ft_strstr(cmd, "./") != NULL)
        {
            ft_putstr_fd_up("minishell:", 2);
            ft_putstr_fd_up(cmd, 2);
            ft_putstr_fd_up(":Is a directory\n", 2);
        }
        else
        {
           handle_cmd_errors(path);
        }
    }
    handle_cmd_errors(path);
    free(path);
    exit(126);
}

static void execute_child(
    t_cmd **cmds, int i, int cmd_count,
    int prev_pipe, int pipes[2],
    char **env, t_env *env_list, t_shell *shell_ctx)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    setup_pipes_in_child(i, cmd_count, prev_pipe, pipes);
    if (redirections(cmds[i], find_last_heredoc_index(cmds[i])) != 0)
        exit(1);
    if (!cmds[i]->cmd || !cmds[i]->cmd[0] || is_spaces(cmds[i]->cmd[0]))
        exit(0);
    if (is_builtin(cmds[i]->cmd[0]))
    {
        env_list = execut_bultin(&cmds[i], env_list, shell_ctx, 0);
        free_env(env);
        if (i == 0)
            exit(shell_ctx->exit_status);
        else
            exit(0);
    }
    execute_external_command(cmds[i]->cmd[0], cmds[i]->cmd, env, i);
}

static void handle_parent_process(int i, int *prev_pipe, int pipes[2], pid_t pid, int cmd_count, t_shell *shell_ctx)
{
    (void)pid;
    (void)shell_ctx;
    if (i > 0 && *prev_pipe != -1)
        close(*prev_pipe);
    if (i < cmd_count - 1)
    {
        close(pipes[1]);
        *prev_pipe = pipes[0];
    }
}

static void update_exit_status(int wstatus, t_shell *shell_ctx)
{
    if (WIFEXITED(wstatus))
        shell_ctx->exit_status = WEXITSTATUS(wstatus);
    else if (WIFSIGNALED(wstatus))
        shell_ctx->exit_status = 128 + WTERMSIG(wstatus);
    else
        shell_ctx->exit_status = 1;
}

static void wait_for_children(pid_t last_pid, t_shell *shell_ctx)
{
    int wstatus = 0;
    pid_t wpid;
    int saw_sigint = 0;

    while ((wpid = wait(&wstatus)) > 0)
    {
        if (WIFSIGNALED(wstatus))
        {
            int sig = WTERMSIG(wstatus);
            if (sig == SIGINT)
                saw_sigint = 1;
            if (sig == SIGQUIT)
                write(1, "Quit: 3\n", 8);
        }

        if (wpid == last_pid)
            update_exit_status(wstatus, shell_ctx);
    }

    if (saw_sigint)
        write(1, "\n", 1);
}

void execute_pipeline(t_cmd **cmds, int cmd_count, char **env, t_env *env_list, t_shell *shell_ctx)
{
    if (!cmds || cmd_count <= 0)
        return;
    if (!process_all_heredocs(cmds, cmd_count, env, shell_ctx))
        return;
    int pipes[2];
    pid_t pid;
    int prev_pipe = -1;
    pid_t last_pid = -1;
    int i = -1;
    ignore_sigint();
    while (++i < cmd_count && cmds[i])
    {
        if (i < cmd_count - 1 && pipe(pipes) == -1)
        {
            perror("minishell: pipe");
            cleanup_heredoc_files(cmds, cmd_count);
            exit(1);
        }
        pid = fork();
        if (pid == 0)
            execute_child(cmds, i, cmd_count, prev_pipe, pipes, env, env_list, shell_ctx);
        else if (pid > 0)
        {
            handle_parent_process(i, &prev_pipe, pipes, pid, cmd_count, shell_ctx);
            last_pid = pid;
        }
        else
        {
            perror("minishell: fork");
            cleanup_heredoc_files(cmds, cmd_count);
            exit(1);
        }
    }
    if (prev_pipe != -1)
        close(prev_pipe);

    wait_for_children(last_pid, shell_ctx);
    cleanup_heredoc_files(cmds, cmd_count);
    restore_sigint();
}
