/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:02:07 by maskour           #+#    #+#             */
/*   Updated: 2025/07/11 22:46:01 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"
static int open_file(char *file, int mode) {
    int fd = 0;
    if (mode == 0)
        fd = open(file, O_RDONLY);
    else if (mode == 1)
        fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (mode == 2)
        fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    return (fd);
}

// ----- Unique temp filename generator -----
static char *get_rundem_name(char *file_name) {
    char *filename;
    int fd;
    char *count_str;
    char *tmp;

    // Use /tmp directory (no need to check, always exists)
    filename = ft_strjoin("/tmp/minishell_", file_name);
    if (!filename) return NULL;
    fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0600);
    if (fd != -1) { close(fd); return filename; }
    free(filename);

    int count = 0;
    while (count < 1000) {
        count_str = ft_itoa(count);
        if (!count_str) return NULL;
        tmp = ft_strjoin(file_name, "_");
        if (!tmp) { free(count_str); return NULL; }
        filename = ft_strjoin(tmp, count_str);
        free(tmp);
        free(count_str);
        if (!filename) return NULL;
        tmp = ft_strjoin("/tmp/minishell_", filename);
        free(filename);
        if (!tmp) return NULL;
        filename = tmp;
        fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0600);
        if (fd != -1) { close(fd); return filename; }
        free(filename);
        count++;
    }
    return NULL;
}
static int cleanup_stdio(int original_stdin, int original_stdout)
{
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
    return 1;
}
// ----- Heredoc: prompts user, writes to temp file, updates file->name -----
// int function_herdoc(t_file *file, char **env, t_shell *shell_ctx)
// {
//     // Generate the temp filename BEFORE fork
//     char *filename = get_rundem_name(file->name);
//     if (!filename) {
//         perror("minishell: cannot create temp file");
//         return 1;
//     }
//     pid_t pid = fork();
//     if (pid == -1) 
//     {
//         free(filename);
//         perror("minishell: fork (heredoc)");
//         return 1;
//     }
//     signal(SIGINT, SIG_IGN);
//     if (pid == 0)
//     {
//         // Child process: set default signal handling
//         signal(SIGINT, SIG_DFL);
//         signal(SIGQUIT, SIG_DFL);

//         int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//         if (fd == -1)
//         {
//             perror("minishell: open");
//             free(filename);
//             exit(1);
//         }

//         char *line;
//         while (1)
//         {
//             line = readline("> ");
//             if (!line)
//                 break; // Normal exit on EOF

//             if (!ft_strcmp(line, file->name))
//             {
//                 free(line);
//                 break;
//             }

//             char *tmp = ft_strjoin("$", file->name);
//             if (file->check_expand == 0 && ft_strcmp(line, tmp) != 0)
//             {
//                 char *expanded_line = found_env(line, env, shell_ctx);
//                 if (!expanded_line)
//                 {
//                     free(line);
//                     close(fd);
//                     unlink(filename);
//                     free(filename);
//                     exit(1);
//                 }
//                 write(fd, expanded_line, ft_strlen(expanded_line));
//                 write(fd, "\n", 1);
//                 free(expanded_line);
//             } 
//             else
//             {
//                 write(fd, line, strlen(line));
//                 write(fd, "\n", 1);
//             }
//             free(tmp);
//             free(line);
//         }
//         close(fd);
//         free(filename); // Not needed in child anymore
//         exit(0); // Success
//     }

//     // Parent: wait for heredoc process
//     int status;
//     waitpid(pid, &status, 0);

//     // Save the filename in the file struct (now parent and child agree)
//     free(file->name);
//     file->name = filename;

//     if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
//         return (130); // Tell caller to cancel pipeline

//     return 2;
// }
int function_herdoc(t_file *file, char **env, t_shell *shell_ctx)
{
    // Generate the temp filename BEFORE fork
    char *filename = get_rundem_name(file->name);
    if (!filename) {
        perror("minishell: cannot create temp file");
        return 1;
    }
    pid_t pid = fork();
    if (pid == -1) 
    {
        free(filename);
        perror("minishell: fork (heredoc)");
        return 1;
    }
    signal(SIGINT, SIG_IGN);
    if (pid == 0)
    {
        // Child process: set default signal handling
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);

        int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
        {
            perror("minishell: open");
            free(filename);
            exit(1);
        }

        char *line;
        while (1)
        {
            line = readline("> ");
            if (!line) // EOF (Ctrl+D)
            {
                // Print warning about EOF without delimiter
                // ft_putstr_fd_up("minishell: warning: here-document delimited by end-of-file (wanted `", 2);
                // ft_putstr_fd_up(file->name, 2);
                // ft_putstr_fd_up("')\n", 2);
                close(fd);
                free(filename); // Exit normally on EOF
                exit(2);
            }

            if (!ft_strcmp(line, file->name))
            {
                free(line);
                break;
            }

            char *tmp = ft_strjoin("$", file->name);
            if (file->check_expand == 0 && ft_strcmp(line, tmp) != 0)
            {
                char *expanded_line = found_env(line, env, shell_ctx);
                if (!expanded_line)
                {
                    free(line);
                    free(tmp);
                    close(fd);
                    unlink(filename);
                    free(filename);
                    exit(1);
                }
                write(fd, expanded_line, ft_strlen(expanded_line));
                write(fd, "\n", 1);
                free(expanded_line);
            } 
            else
            {
                write(fd, line, strlen(line));
                write(fd, "\n", 1);
            }
            free(tmp);
            free(line);
        }
        close(fd);
        free(filename);
        exit(0); // Always exit 0 for successful heredoc completion
    }

    // Parent: wait for heredoc process
    int status;
    waitpid(pid, &status, 0);

    // Check if child was interrupted by SIGINT (Ctrl+C)
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
        // Clean up the temp file
        unlink(filename);
        free(filename);
        return 130; // Return 130 to indicate SIGINT interruption
    }

    // If child exited normally (either EOF or delimiter found)
    if (WIFEXITED(status) && WEXITSTATUS(status) == 2)
    {
        // If we reached here, it means heredoc was completed successfully
        // Save the filename in the file struct (now parent and child agree)
        free(file->name);
        file->name = filename;
        return 2; // Success - heredoc completed normally
    }
    else if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
    {
        // Save the filename in the file struct
        free(file->name);
        file->name = filename;
        return 0; // Success - heredoc completed normally
    }

    // Other error cases
    unlink(filename);
    free(filename);
    return 0;
}
// ----- Redirection logic -----
int redirections(t_cmd *cmd, int last_heredoc_index)
{
    if (!cmd || cmd->file_count <= 0)
        return 0;

    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);
    if (original_stdin == -1 || original_stdout == -1) {
        perror("minishell: dup");
        return 1;
    }
    int last_in_fd = -1;
    int last_out_fd = -1;
    // int last_heredoc_index = -1;
    // int i = -1;
    // Handle heredocs
    // while (++i < cmd->file_count)
    // {
    //     if (cmd->files[i].type == TOKEN_HEREDOC) {
    //     int hd_status = function_herdoc(&cmd->files[i], env, shell);
    //         if (hd_status != 0)
    //         {
    //             cleanup_stdio(original_stdin, original_stdout);
    //         }
    //         last_heredoc_index = i;
    //     }
    // }
    int i = -1;
    while (++i < cmd->file_count)
    {
        t_file *file = &cmd->files[i];
        if (file->type == TOKEN_REDIRECT_IN) {
            if (last_in_fd != -1) close(last_in_fd);
            last_in_fd = open_file(file->name, 0);
            if (last_in_fd == -1) {
                perror("minishell: open_file input");
                cleanup_stdio(original_stdin, original_stdout);
                return 1;
            }
        }
        else if (file->type == TOKEN_HEREDOC && i == last_heredoc_index) {
            if (last_in_fd != -1) close(last_in_fd);
            last_in_fd = open(file->name, O_RDONLY);
            if (last_in_fd == -1) {
                perror("minishell: open heredoc file");
                cleanup_stdio(original_stdin, original_stdout);
                return 1;
            }
        }
        else if (file->type == TOKEN_REDIRECT_OUT) {
            if (last_out_fd != -1) close(last_out_fd);
            last_out_fd = open_file(file->name, 1);
            if (last_out_fd == -1) {
                perror("minishell: open_file output");
                cleanup_stdio(original_stdin, original_stdout);
                return 1;
            }
        }
        else if (file->type == TOKEN_APPEND) {
            if (last_out_fd != -1) close(last_out_fd);
            last_out_fd = open_file(file->name, 2);
            if (last_out_fd == -1) {
                perror("minishell: open_file append");
                cleanup_stdio(original_stdin, original_stdout);
                return 1;
            }
        }
    }
    if (last_in_fd != -1)
    {
        if (dup2(last_in_fd, STDIN_FILENO) == -1)
        {
            perror("minishell: dup2 input");
            close(last_in_fd);
            cleanup_stdio(original_stdin, original_stdout);
            return 1;
        }
        close(last_in_fd);
    }
    if (last_out_fd != -1)
    {
        if (dup2(last_out_fd, STDOUT_FILENO) == -1)
        {
            perror("minishell: dup2 output");
            close(last_out_fd);
            cleanup_stdio(original_stdin, original_stdout);
            return 1;
        }
        close(last_out_fd);
    }
    close(original_stdin);
    close(original_stdout);
    return 0;
}
