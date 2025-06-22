/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:01:55 by maskour           #+#    #+#             */
/*   Updated: 2025/06/21 22:36:29 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"
static void ignore_sigint(void)
{
    signal(SIGINT, SIG_IGN);
}

static void restore_sigint(void)
{
    signal(SIGINT, handler_sig);
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
char **convert(t_env *env_list)
{
    int count = 0;
    int i = -1;
    char **env_arry;
    t_env *current = env_list;
    if (!env_list)
    {
        env_arry = malloc(sizeof(char *) * 1);
        if (!env_arry)
            return NULL;
        env_arry[0] = NULL;
        return env_arry;
    }
    // Count environment variables
    while (current)
    {
        count++;
        current = current->next;
    }

    env_arry = malloc (sizeof(char *) * (count + 1));
    if (!env_arry)
        return (NULL);
    current = env_list;
    while (++i < count && current)
    {
        env_arry[i] = ft_strdup(current->data_env); // Make a copy instead of direct assignment
        if (!env_arry[i])
        {
            // Cleanup if allocation fails
            while (--i >= 0)
                free(env_arry[i]);
            free(env_arry);
            return (NULL);
        }
        current = current->next;
    }
    env_arry[count] = NULL;
    return (env_arry);
}
void	ft_putstr_fd_up(char *s, int fd)
{
	size_t	len;

	if (!s || fd < 0)
		return ;
	len = 0;
	while (s[len])
		len++;
	write(fd, s, len);
}

static void handle_cmd_errors(char *cmd_path)
{
    if(cmd_path)
	{
	    ft_putstr_fd_up("minishell: 1", 2);
        ft_putstr_fd_up(cmd_path, 2);
        ft_putstr_fd_up(": execution failed", 2);
	}
}

static void cmd_process(t_cmd *cmd, char **env)
{
    char *cmd_path;
    if (!cmd)
    { 
        handle_cmd_errors(NULL);
        exit(1); // Exit with error
    }
    if (redirections(cmd))
        exit(1); // Error in redirection

    if (!cmd->cmd || !cmd->cmd[0])
    {   
        handle_cmd_errors(NULL);
        exit(1); // Exit with error
    }
    cmd_path = find_path(cmd->cmd[0], env);
    if (!cmd_path)
    {
        ft_putstr_fd_up("minishell:", 2);
        ft_putstr_fd_up(cmd->cmd[0], 2);
        ft_putstr_fd_up(": command not found\n", 2);
        exit(127); // Command not found exit code
    }
    if (execve(cmd_path, cmd->cmd, env) == -1)
    {
        handle_cmd_errors(cmd_path);
        free(cmd_path);
        exit(126); // Cannot execute
    }
    exit(0); // Should never reach here
}

static void execute_single_command(t_cmd **cmd, char **envp, t_shell *shell_ctx)
{
    pid_t id;
    int status;
    
    ignore_sigint();  // Ignore SIGINT in parent while child is running
    id = fork();
    if (id == 0)
    {
        // Child process - restore default SIGINT handler
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        cmd_process(*cmd, envp);
        shell_ctx->exit_status = 0;
    }
    else if (id > 0)
    {
        waitpid(id, &status, 0);
        restore_sigint();  // Restore SIGINT handler in parent
        if (WIFEXITED(status))
            shell_ctx->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            shell_ctx->exit_status = 128 + WTERMSIG(status);
        else
            shell_ctx->exit_status = 1;
    }
    else
    {
        perror("fork failed");
        restore_sigint();
    }
}
static void execute_pipeline(t_cmd **cmds, int cmd_count, char **env, t_shell *shell_ctx)
{
    if (!cmds || cmd_count <= 0)
        return;

    int pipes[2];
    pid_t pid;
    int prev_pipe = -1;
    pid_t last_pid = -1;
    int i = 0;

    while (i < cmd_count && cmds[i] != NULL)
    {
        if (i < cmd_count - 1 && pipe(pipes) == -1) 
        {
            perror("minishell: pipe");
            exit(1);
        }

        pid = fork();
        if (pid == 0) 
        {
            // Child process
            signal(SIGQUIT, handler_sig);
            signal(SIGINT, handler_sig);

            // Set up input from previous command
            if (i > 0) {
                if (dup2(prev_pipe, STDIN_FILENO) == -1) {
                    perror("minishell: dup2");
                    exit(1);
                }
                close(prev_pipe);
            }

            // Set up output to next command
            if (i < cmd_count - 1) {
                close(pipes[0]);
                if (dup2(pipes[1], STDOUT_FILENO) == -1) {
                    perror("minishell: dup2");
                    exit(1);
                }
                close(pipes[1]);
            }

            // Validate command
            if (!cmds[i]->cmd || !cmds[i]->cmd[0]) {
                ft_putstr_fd_up("minishell: empty command\n", 2);
                exit(127);
            }
            // Handle redirections
            if (redirections(cmds[i]))
            {
                exit(1);  // Redirection failed
            }
            // Find and execute command
            char *path = find_path(cmds[i]->cmd[0], env);
            if (!path) {
                ft_putstr_fd_up("minishell: ", 2);
                ft_putstr_fd_up(cmds[i]->cmd[0], 2);
                ft_putstr_fd_up(": command not found\n", 2);
                exit(127);
            }
            // If execve succeeds, it won't return
            if (execve(path, cmds[i]->cmd, env) == -1)
            {
             handle_cmd_errors(path);
             free(path);
             exit(126);
            }
            // If we get here, execve failed
            perror("minishell: execve");
            free(path);
            exit(126);
        }
        else if (pid > 0) 
        {
            // Parent process
            if (i > 0)
                close(prev_pipe);
            if (i < cmd_count - 1) 
            {
                close(pipes[1]);
                prev_pipe = pipes[0];
            }
            last_pid = pid;
        }
        else 
        {
            perror("minishell: fork");
            exit(1);
        }
        i++;
    }

    // Clean up any remaining pipe ends
    if (prev_pipe != -1)
        close(prev_pipe);

    // Wait for all child processes
    int wstatus = 0;
    pid_t wpid;
    while ((wpid = wait(&wstatus)) > 0) {
        if (wpid == last_pid) {
            if (WIFEXITED(wstatus))
                shell_ctx->exit_status = WEXITSTATUS(wstatus);
            else if (WIFSIGNALED(wstatus))
                shell_ctx->exit_status = 128 + WTERMSIG(wstatus);
            else
                shell_ctx->exit_status = 1;
        }
    }
}
int exicut(t_cmd **cmd, t_env **env_list, t_shell *shell_ctx)
{
    int cmd_count = 0;
    if (!cmd || !*cmd || !env_list)
        return (1);
    t_cmd *current = *cmd;
    char **env = convert(*env_list);
    if (!env)
        return (1);

    // Count commands
    while (current)
    {
        cmd_count++;
        current = current->next;
    }

    // Single command branch
    if (cmd_count == 1)
    {
        if (is_builtin((*cmd)->cmd[0]))
        {
            *env_list = execut_bultin(cmd, *env_list, shell_ctx);
            free_env(env);
            return (0);
        }
        execute_single_command(cmd, env, shell_ctx);
    }
    else
    {
        // Convert linked list to array
        t_cmd **cmd_arr = malloc(sizeof(t_cmd *) * (cmd_count + 1));
        if (!cmd_arr) {
            free_env(env);
            return (1);
        }
        current = *cmd;
        int i = -1;
        while ( ++i < cmd_count) 
        {
            cmd_arr[i] = current;
            current = current->next;
        }
        cmd_arr[cmd_count] = NULL;
        execute_pipeline(cmd_arr, cmd_count, env, shell_ctx);
        free(cmd_arr);
    }
    free_env(env);
    return (0);
}
