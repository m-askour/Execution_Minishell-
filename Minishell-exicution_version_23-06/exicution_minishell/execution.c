/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:01:55 by maskour           #+#    #+#             */
/*   Updated: 2025/07/01 20:29:52 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"
// static void handl_quitsig(int signal)
// {
//     if (signal == SIGINT)
// 	{
//     	 write(1,"\n",1);
//   		rl_on_new_line();
//   		rl_replace_line("", 0);
//   		rl_redisplay();
// 	}
// }
static void ignore_sigint(void)
{
    signal(SIGINT, SIG_IGN);
}

static void restore_sigint(void)
{
    // write(1,"\n",1);
    signal(SIGINT, handler_sig);
}
// static void restore_sigint_1(void)
// {
//     signal(SIGINT, SIG_IGN);
//     write(1,"Quit\n",5);
// }

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
        ft_putstr_fd_up(": execution failed\n", 2);
	}
}

static void cmd_process(t_cmd *cmd, char **env)
{
    char *cmd_path;
    if (!cmd || !cmd->cmd) 
    {
        ft_putstr_fd_up("minishell:", 2);
        ft_putstr_fd_up(" command not found\n", 2);
        exit(127);
    }
    if (redirections(cmd))
        exit(1); // Error in redirection
    if (!cmd->cmd || !cmd->cmd[0])
    {   
        handle_cmd_errors(NULL);
        exit(1); // Exit with error
    }

    cmd_path = find_path(cmd->cmd[0], env);
    // printf("%s\n",cmd_path);
    if (!cmd_path)
    {
       if (access(cmd->cmd[0], F_OK) == 0)
        {
            ft_putstr_fd_up("minishell: ", 2);
            ft_putstr_fd_up(cmd->cmd[0], 2);
            ft_putstr_fd_up(": Permission denied\n", 2);
            exit(126); // Permission denied exit code
        }
        ft_putstr_fd_up("minishell:", 2);
        ft_putstr_fd_up(cmd->cmd[0], 2);
        ft_putstr_fd_up(": command not found\n", 2);
        exit(127); // Command not found exit code
    }
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
        exit(126); // Cannot execute
    }
    exit(0); // Should never reach here
}

static void execute_single_command(t_cmd **cmd, char **envp, t_shell *shell_ctx)
{
    pid_t id;
    int status;
    struct termios original_termios;
    
    // Save current terminal attributes
    if (tcgetattr(STDIN_FILENO, &original_termios) == -1) {
        perror("tcgetattr failed");
        return;
    }

    ignore_sigint();  // Ignore SIGINT in parent while child is running
    
    id = fork();
    if (id == 0)
    {
        // Child process - restore default SIGINT handler
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        
        // Set terminal attributes for child process if needed
        // Modify child_termios settings if needed (e.g., disable echo, etc.)
        // tcsetattr(STDIN_FILENO, TCSANOW, &child_termios);
        
        cmd_process(*cmd, envp);
        shell_ctx->exit_status = 0;
        
        // Child exits - no need to restore original attributes as they're per-process
        exit(0);
    }
    else if (id > 0)
    {
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        waitpid(id, &status, 0);

        // Restore original terminal attributes in parent
        if (tcsetattr(STDIN_FILENO, TCSANOW, &original_termios) == -1) {
            perror("tcsetattr failed");
        }

        restore_sigint();  // Restore SIGINT handler in parent

        if (WIFEXITED(status))
            shell_ctx->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
        {
            shell_ctx->exit_status = 128 + WTERMSIG(status);
            if (WTERMSIG(status) == SIGINT)
            {
                    write(1, "\n", 2);
            }  // Typically just newline for SIGINT
            else if (WTERMSIG(status) == SIGQUIT)
                write(1, "Quit\n", 5);
        }
        else
            shell_ctx->exit_status = 1;
    }
    else
    {
        perror("fork failed");
        // Restore terminal attributes if fork fails
        if (tcsetattr(STDIN_FILENO, TCSANOW, &original_termios) == -1) {
            perror("tcsetattr failed");
        }
        restore_sigint();
    }
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
    for (int j = 0; j < cmd_count; j++)
    {
        t_cmd *cmd = cmds[j];
        for (int k = 0; k < cmd->file_count; k++)
        {
            if (cmd->files[k].type == TOKEN_HEREDOC)
                unlink(cmd->files[k].name);
        }
    }
}
static int handle_redirections(t_cmd *cmd)
{
    for (int i = 0; i < cmd->file_count; i++)
    {
        t_file *file = &cmd->files[i];
        
        if (file->type == TOKEN_HEREDOC)
        {
            // Open the temp file created during preprocessing
            int fd = open(file->name, O_RDONLY);
            if (fd == -1)
            {
                perror("minishell: heredoc file");
                return 1;
            }
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("minishell: dup2");
                close(fd);
                return 1;
            }
            close(fd);
        }
        // Handle other redirection types here...
    }
    return 0;
}

static void execute_pipeline(t_cmd **cmds, int cmd_count, char **env, t_env *env_list, t_shell *shell_ctx)
{
    if (!cmds || cmd_count <= 0)
        return;
        
    // 1. === PRE-PROCESS: Collect heredoc input for all commands ===
    // (DO THIS BEFORE ANY FORKING!)
    for (int h = 0; h < cmd_count; h++)
    {
        for (int f = 0; f < cmds[h]->file_count; f++)
        {
            if (cmds[h]->files[f].type == TOKEN_HEREDOC)
            {
                int hd_status = function_herdoc(&cmds[h]->files[f]);
                if (hd_status == 130)
                {
                    shell_ctx->exit_status = 1;
                    // Clean up any temp files created so far
                    cleanup_heredoc_files(cmds, cmd_count);
                    return; // Abort pipeline on Ctrl+C
                }
                else if (hd_status != 0)
                {
                    shell_ctx->exit_status = 1;
                    cleanup_heredoc_files(cmds, cmd_count);
                    return; // Other heredoc failure
                }
            }
        }
    }
    
    int pipes[2];
    pid_t pid;
    int prev_pipe = -1;
    pid_t last_pid = -1;
    int i = 0;

    // Ignore SIGINT in parent during pipeline execution
    ignore_sigint();

    while (i < cmd_count && cmds[i] != NULL)
    {
        if (i < cmd_count - 1 && pipe(pipes) == -1)
        {
            perror("minishell: pipe");
            cleanup_heredoc_files(cmds, cmd_count);
            exit(1);
        }

        pid = fork();
        if (pid == 0)
        {
            // Child process
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            
            // If not the first command, set up input from previous pipe
            if (i > 0)
            {
                if (dup2(prev_pipe, STDIN_FILENO) == -1)
                {
                    perror("minishell: dup2");
                    exit(1);
                }
                close(prev_pipe);
            }

            // If not the last command, set up output to next pipe
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

            // Handle file redirections (including heredoc)
            if (handle_redirections(cmds[i]) != 0)
                exit(1);
        
            // Validate command
            if (!cmds[i]->cmd || !cmds[i]->cmd[0] || is_spaces(cmds[i]->cmd[0]))
            {
                ft_putstr_fd_up("minishell: command not found\n", 2);
                exit(127);
            }

            // Handle builtins in child process (pipeline)
            if (is_builtin(cmds[i]->cmd[0]))
            {
                env_list = execut_bultin(&cmds[i], env_list, shell_ctx, 0);
                free_env(env);
                exit(shell_ctx->exit_status);
            }

            // External command
            char *path = find_path(cmds[i]->cmd[0], env);
            if (!path)
            {
                if (access(cmds[i]->cmd[0], F_OK) == 0)
                {
                    ft_putstr_fd_up("minishell: ", 2);
                    ft_putstr_fd_up(cmds[i]->cmd[0], 2);
                    ft_putstr_fd_up(": Permission denied\n", 2);
                    exit(126);
                }
                ft_putstr_fd_up("minishell: ", 2);
                ft_putstr_fd_up(cmds[i]->cmd[0], 2);
                ft_putstr_fd_up(": 2command not found\n", 2);
                exit(127);
            }
            
            if (execve(path, cmds[i]->cmd, env) == -1)
            {
                handle_cmd_errors(path);
                free(path);
                exit(126);
            }
            free(path);
            exit(126);
        }
        else if (pid > 0)
        {
            signal(SIGQUIT, SIG_IGN);
            if (i > 0 && prev_pipe != -1)
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
            cleanup_heredoc_files(cmds, cmd_count);
            exit(1);
        }
        i++;
    }
    
    if (prev_pipe != -1)
        close(prev_pipe);
        
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
        {
            if (WIFEXITED(wstatus))
                shell_ctx->exit_status = WEXITSTATUS(wstatus);
            else if (WIFSIGNALED(wstatus))
                shell_ctx->exit_status = 128 + WTERMSIG(wstatus);
            else
                shell_ctx->exit_status = 1;
        }
    }

    if (saw_sigint)
        write(1, "\n", 1);

    // Clean up heredoc temp files
    cleanup_heredoc_files(cmds, cmd_count);
    restore_sigint();
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
            *env_list = execut_bultin(cmd, *env_list, shell_ctx, 1);
            free_env(env);
            return (0);
        }
        execute_single_command(cmd, env,shell_ctx);
    }
    else
    {// Convert linked list to array
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
        execute_pipeline(cmd_arr, cmd_count, env, *env_list,shell_ctx);
        free(cmd_arr);
    }
    free_env(env);
    return (0);
}
