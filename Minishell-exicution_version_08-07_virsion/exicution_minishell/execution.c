/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:01:55 by maskour           #+#    #+#             */
/*   Updated: 2025/07/11 23:01:07 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

static void ignore_sigint(void)
{
    signal(SIGINT, SIG_IGN);
}

static void restore_sigint(void)
{
    // write(1,"\n",1);
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
        env_arry[i] = ft_strdup(current->data_env); 
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
static int cleanup_stdio(int original_stdin, int original_stdout)
{
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
    return 1;
}

static int cmd_process(t_cmd *cmd, char **env, t_shell *shell)
{
    char *cmd_path;
    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);
    int last_heredoc_index = -1;
    int i = -1;

    // --- HEREDOC HANDLING ---
    while (++i < cmd->file_count)
    {
        if (cmd->files[i].type == TOKEN_HEREDOC) {
            int hd_status = function_herdoc(&cmd->files[i], env, shell);
            if (hd_status == 130) // SIGINT during heredoc
            {
                cleanup_stdio(original_stdin, original_stdout);
                exit(130); // Exit with SIGINT status
            }
            else if (hd_status != 0) // Other heredoc errors
            {
                cleanup_stdio(original_stdin, original_stdout);
                exit(hd_status);
            }
            else if (hd_status == 2) // Heredoc was cancelled
            {
                cleanup_stdio(original_stdin, original_stdout);
                exit( 2); // Indicate cancellation
            }
            printf("Heredoc processed successfully%d\n", hd_status);
            last_heredoc_index = i;
        }
    }
    // --- END HEREDOC HANDLING ---

    if (!cmd || !cmd->cmd) 
    {
        ft_putstr_fd_up("minishell:", 2);
        ft_putstr_fd_up(" command not found\n", 2);
        exit(127);
    }
    int redir_status = redirections(cmd, last_heredoc_index);
    if (redir_status != 0)
        exit(0);

    if (!cmd->cmd || !cmd->cmd[0])
    {   
        handle_cmd_errors(NULL);
        exit(0);
    }

    cmd_path = find_path(cmd->cmd[0], env);
    if (!cmd_path)
    {
        if (access(cmd->cmd[0], F_OK) == 0)
        {
            ft_putstr_fd_up("minishell: ", 2);
            ft_putstr_fd_up(cmd->cmd[0], 2);
            ft_putstr_fd_up(": Permission denied\n", 2);
            exit(126);
        }
        if (ft_strstr(cmd->cmd[0], "./") != NULL) 
        {
            ft_putstr_fd_up("minishell:", 2);
            ft_putstr_fd_up(cmd->cmd[0], 2);
            ft_putstr_fd_up("No such file or directory\n", 2);
        }
        else
        {
            ft_putstr_fd_up("minishell:", 2);
            ft_putstr_fd_up(cmd->cmd[0], 2);
            ft_putstr_fd_up(": command not found\n", 2);
        }
        exit(127);
    }
    if (execve(cmd_path, cmd->cmd, env) == -1)
    {
        if (cmd->cmd[0][0] == '\0') 
        {
            ft_putstr_fd_up("minishell:", 2);
            ft_putstr_fd_up(" command not found\n", 2);
            exit(127);
        }
        if (ft_strstr(cmd->cmd[0], "./") != NULL) 
        {
            ft_putstr_fd_up("minishell:", 2);
            ft_putstr_fd_up(cmd->cmd[0], 2);
            ft_putstr_fd_up("Is a directory\n", 2);
        }
        else 
            handle_cmd_errors(cmd_path);
        free(cmd_path);
        exit(126);
    }
    exit(0);
}

// Modified execute_single_command to handle heredoc SIGINT properly
static void execute_single_command(t_cmd **cmd, char **envp, t_shell *shell_ctx)
{
    pid_t id;
    int status;
    struct termios original_termios;
    // int cmd_int = 0;

    // Save current terminal attributes
    if (tcgetattr(STDIN_FILENO, &original_termios) == -1) {
        perror("tcgetattr failed");
        return;
    }

    ignore_sigint();
    // if ((*cmd)->cmd && (*cmd)->cmd[0] && ft_strncmp((*cmd)->cmd[0], "cat", 4) == 0)
    //     cmd_int = 1;
    // else
    //     cmd_int = 0;
    
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
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        waitpid(id, &status, 0);

        if (tcsetattr(STDIN_FILENO, TCSANOW, &original_termios) == -1) {
            perror("tcsetattr failed");
        }

        restore_sigint();

        if (WIFEXITED(status))
        {
            int child_exit = WEXITSTATUS(status);
            if (child_exit == 130) // SIGINT during heredoc
            {
                shell_ctx->exit_status = 1;
                write(1, "\n", 1);
                return;
            }
            else if (child_exit == 2)
            {
                shell_ctx->exit_status = 0;
                return;
            }
            shell_ctx->exit_status = child_exit;
        }
        else if (WIFSIGNALED(status))
        {
            shell_ctx->exit_status = 128 + WTERMSIG(status);
            if (WTERMSIG(status) == SIGINT)
            {
                    write(1,"\n",1);
            }
            else if (WTERMSIG(status) == SIGQUIT)
                write(1, "Quit\n", 5);
        }
        else
            shell_ctx->exit_status = 0;
    }
    else
    {
        perror("fork failed");
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
static int handle_redirections(t_cmd *cmd)
{
    int i = -1;
    int fd;

    while (++i < cmd->file_count)
    {
        t_file *file = &cmd->files[i];
        if (file->type == TOKEN_HEREDOC || file->type == TOKEN_REDIRECT_IN) // handle heredoc and input '<'
        {
            fd = open(file->name, O_RDONLY);
            if (fd == -1)
            {
                perror("minishell: input file");
                return 1;
            }
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("minishell: dup2 input");
                close(fd);
                return 1;
            }
            close(fd);
        }
        else if (file->type == TOKEN_REDIRECT_OUT) // handle output '>'
        {
            fd = open(file->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                perror("minishell: output file");
                return 1;
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("minishell: dup2 output");
                close(fd);
                return 1;
            }
            close(fd);
        }
        else if (file->type == TOKEN_APPEND) // handle append '>>'
        {
            fd = open(file->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1)
            {
                perror("minishell: append file");
                return 1;
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("minishell: dup2 append");
                close(fd);
                return 1;
            }
            close(fd);
        }
    }
    return 0;
}

static void execute_pipeline(t_cmd **cmds, int cmd_count, char **env, t_env *env_list, t_shell *shell_ctx)
{
    if (!cmds || cmd_count <= 0)
        return;
    int h = -1;
    int f;
    while (++h < cmd_count)
    {
        f = -1;
        while (++f < cmds[h]->file_count)
        {
            if (cmds[h]->files[f].type == TOKEN_HEREDOC)
            {
                int hd_status = function_herdoc(&cmds[h]->files[f], env, shell_ctx);
                if (hd_status == 130)
                {
                    shell_ctx->exit_status = 1; // Set to 130 instead of 1
                    // Clean up any temp files created so far
                    cleanup_heredoc_files(cmds, cmd_count);
                    signal(SIGINT, SIG_IGN);
                    return; // Abort pipeline on Ctrl+C
                }
                else if (hd_status != 0)
                {
                    shell_ctx->exit_status = 1;
                    cleanup_heredoc_files(cmds, cmd_count);
                    signal(SIGINT, SIG_IGN);
                    return; // Other heredoc failure
                }
            }
            signal(SIGINT, SIG_IGN);
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
                exit(0);
            }
            if (is_builtin(cmds[i]->cmd[0]))
            {
                env_list = execut_bultin(&cmds[i],env_list, shell_ctx, 0);
                free_env(env);
                if (i == 0)
                    exit(shell_ctx->exit_status);
                else 
                    exit(0);
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
                if (i == 0)
                {   
		    if (ft_strstr(cmds[i]->cmd[0], "./") != NULL)
                    {
                        ft_putstr_fd_up("minishell:", 2);
                        ft_putstr_fd_up(cmds[i]->cmd[0], 2);
                        ft_putstr_fd_up(":No such file or directory\n", 2);
                    }
                    else
                    {
                        ft_putstr_fd_up("minishell:", 2);
                        ft_putstr_fd_up(cmds[i]->cmd[0], 2);
                        ft_putstr_fd_up(": command not found\n", 2);
                    }
		}
                else
                {
                    ft_putstr_fd_up("minishell: 0", 2);
                    ft_putstr_fd_up(": command not found\n", 2);    
                }
                exit(127);  // all others return 0
            }
            
            if (execve(path, cmds[i]->cmd, env) == -1)
            {
                if (ft_strstr(cmds[i]->cmd[0], "./") != NULL)
                {
                    ft_putstr_fd_up("minishell:", 2);
                    ft_putstr_fd_up(cmds[i]->cmd[0], 2);
                    ft_putstr_fd_up(":Is a directory\n", 2);
                }
                else
                {
                   handle_cmd_errors(path);
                }
                free(path);
                exit(126);
            }
            free(path);
            exit(126);
        }
        else if (pid > 0)
        {
            printf("exit_sttatus: %d\n", shell_ctx->exit_status);
            // signal(SIGQUIT, SIG_IGN);
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
            {
                    shell_ctx->exit_status = 128 + WTERMSIG(wstatus);
            }
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
//norminite
// static int count_cmd(t_cmd *cmd)
// {
//     int count;
    
//     count = 0;
//     while (cmd)
//     {
//         count++;
//         cmd = cmd->next;
//     }
//     return (count);
// }
// static t_cmd **convert_lit_arr(t_cmd *cmd, int cmd_count)
// {
//         t_cmd    *current = cmd;
//         t_cmd **cmd_arr = malloc(sizeof(t_cmd *) * (cmd_count + 1));
//         if (!cmd_arr)
//             return (NULL);
//         int i = -1;
//         while ( ++i < cmd_count) 
//         {
//             cmd_arr[i] = current;
//             current = current->next;
//         }
//         cmd_arr[cmd_count] = NULL;
//         return (cmd_arr);
// }
// static void execute_single(t_cmd **cmd, char **env, t_env **env_list, t_shell *shell_ctx)
// {
//         if (is_builtin((*cmd)->cmd[0]))
//             *env_list = execut_bultin(cmd, env,*env_list, shell_ctx, 1);
//         else
//             execute_single_command(cmd, env,shell_ctx);
// }
// int exicut(t_cmd **cmd, t_env **env_list, t_shell *shell_ctx)
// {
//     int cmd_count;
//     t_cmd **cmd_arr;
//     char **env;

//     if (!cmd || !*cmd || !env_list)
//         return (1);
//     env = convert(*env_list);
//     if (!env)
//         return (1);
//     cmd_count = count_cmd(*cmd);
//     if (cmd_count == 1)
//         execute_single(cmd, env, env_list, shell_ctx);
//     else
//     {
//         cmd_arr = convert_lit_arr(*cmd, cmd_count);
//         if (!cmd_arr) {
//             free_env(env);
//             return (1);
//         }
//         execute_pipeline(cmd_arr, cmd_count, env, *env_list,shell_ctx);
//         free(cmd_arr);
//     }
//     free_env(env);
//     return (0);
// }
