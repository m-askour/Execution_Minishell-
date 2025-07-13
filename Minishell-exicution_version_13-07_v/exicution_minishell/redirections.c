/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:02:07 by maskour           #+#    #+#             */
/*   Updated: 2025/07/14 00:10:17 by maskour          ###   ########.fr       */
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
// static char *build_tmp_path(char *prefix, char *file_name)
// // {
// //     char *tmp = ft_strjoin(prefix, file_name);
// //     return tmp; // caller must check NULL
// // }
// // static int try_create_file(char *filename)
// // {
// //     int fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0600);
// //     if (fd != -1)
// //         close(fd);
// //     return fd;
// // }
// // static char *build_numbered_name(char *file_name, int count)
// // {
// //     char *count_str = ft_itoa(count);
// //     if (!count_str)
// //         return NULL;

// //     char *tmp = ft_strjoin(file_name, "_");
// //     if (!tmp)
// //     {
// //         free(count_str);
// //         return NULL;
// //     }

// //     char *numbered = ft_strjoin(tmp, count_str);
// //     free(tmp);
// //     free(count_str);
// //     return numbered;
// // }
// // static char *build_full_numbered_path(char *numbered_name)
// // {
// //     char *tmp = ft_strjoin("/tmp/minishell_", numbered_name);
// //     return tmp; // caller must check NULL
// // }
// // static char *get_rundem_name(char *file_name)
// // {
// //     char *filename = build_tmp_path("/tmp/minishell_", file_name);
// //     if (!filename)
// //         return NULL;

// //     if (try_create_file(filename) != -1)
// //         return filename;

// //     free(filename);
// //     int count = -1;
// //     while ( ++count < 1000)
// //     {
// //         char *numbered_name = build_numbered_name(file_name, count);
// //         if (!numbered_name)
// //             return NULL;
// //         char *full_path = build_full_numbered_path(numbered_name);
// //         free(numbered_name);
// //         if (!full_path)
// //             return NULL;
// //         if (try_create_file(full_path) != -1)
// //             return full_path;
// //         free(full_path);
// // //     }
// //     return NULL;
// // }
// // ----- Unique temp filename generator -----
// static char *get_rundem_name(char *file_name) {
//     char *filename;
//     int fd;
//     char *count_str;
//     char *tmp;

//     filename = ft_strjoin("/tmp/minishell_", file_name);
//     if (!filename) return NULL;
//     fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0600);
//     if (fd != -1) { close(fd); return filename; }
//     free(filename);

//     int count = 0;
//     while (count < 1000) {
//         count_str = ft_itoa(count);
//         if (!count_str) return NULL;
//         tmp = ft_strjoin(file_name, "_");
//         if (!tmp) { free(count_str); return NULL; }
//         filename = ft_strjoin(tmp, count_str);
//         free(tmp);
//         free(count_str);
//         if (!filename) return NULL;
//         tmp = ft_strjoin("/tmp/minishell_", filename);
//         free(filename);
//         if (!tmp) return NULL;
//         filename = tmp;
//         fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0600);
//         if (fd != -1) { close(fd); return filename; }
//         free(filename);
//         count++;
//     }
//     return NULL;
// }
static int cleanup_stdio(int original_stdin, int original_stdout)
{
    int ret = 0;
    
    if (dup2(original_stdin, STDIN_FILENO) == -1) {
        perror("minishell: dup2 stdin restore failed");
        ret = 1;
    }
    if (dup2(original_stdout, STDOUT_FILENO) == -1) {
        perror("minishell: dup2 stdout restore failed");
        ret = 1;
    }
    
    close(original_stdin);
    close(original_stdout);
    return ret;
}

// int function_herdoc(t_file *file, char **env, t_shell *shell_ctx)
// {
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
//             {
//                 close(fd);
//                 free(filename);
//                 exit(2);
//             }

//             if (!ft_strcmp(line, file->name))
//             {
//                 free(line);
//                 break;
//             }

//             if (file->check_expand == 0 && ft_strcmp(line, file->name) != 0)
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
//             free(line);
//         }
//         close(fd);
//         free(filename);
//         exit(0); // Always exit 0 for successful heredoc completion
//     }

//     // Parent: wait for heredoc process
//     int status;
//     waitpid(pid, &status, 0);

//     // Check if child was interrupted by SIGINT (Ctrl+C)
//     if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
//     {
//         // Clean up the temp file
//         unlink(filename);
//         free(filename);
//         return 130; // Return 130 to indicate SIGINT interruption
//     }

//     // If child exited normally (either EOF or delimiter found)
//     if (WIFEXITED(status) && WEXITSTATUS(status) == 2)
//     {
//         // If we reached here, it means heredoc was completed successfully
//         // Save the filename in the file struct (now parent and child agree)
//         free(file->name);
//         file->name = filename;
//         return 2; // Success - heredoc completed normally
//     }
//     else if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
//     {
//         // Save the filename in the file struct
//         free(file->name);
//         file->name = filename;
//         return 0; // Success - heredoc completed normally
//     }

//     // Other error cases
//     unlink(filename);
//     free(filename);
//     return 0;
// }
// ----- Redirection logic -----
static int is_ambiguous_redirect(const char *filename)
{
    if (!filename)
        return 1;
    if (filename[0] == '\0')
        return 1;
    if (ft_strcmp(filename, "\2") == 0)
        return 1;
    return 0;
}
// int redirections(t_cmd *cmd, int last_heredoc_index)
// {
//     if (!cmd || cmd->file_count <= 0)
//         return 0;

//     int original_stdin = dup(STDIN_FILENO);
//     int original_stdout = dup(STDOUT_FILENO);
//     if (original_stdin == -1 || original_stdout == -1) {
//         perror("minishell: dup");
//         return 1;
//     }
//     int last_in_fd = -1;
//     int last_out_fd = -1;
//     // int last_heredoc_index = -1;
//     // int i = -1;
//     // Handle heredocs
//     // while (++i < cmd->file_count)
//     // {
//     //     if (cmd->files[i].type == TOKEN_HEREDOC) {
//     //     int hd_status = function_herdoc(&cmd->files[i], env, shell);
//     //         if (hd_status != 0)
//     //         {
//     //             cleanup_stdio(original_stdin, original_stdout);
//     //         }
//     //         last_heredoc_index = i;
//     //     }
//     // }
//     int i = -1;
//     while (++i < cmd->file_count)
//     {
//         t_file *file = &cmd->files[i];
//         if (is_ambiguous_redirect(file->name))
//         {
//             ft_putstr_fd_up("minishell: ", 2);
//             ft_putstr_fd_up(file->name, 2); 
//             ft_putstr_fd(": ambiguous redirect\n", 2, 0);
//             cleanup_stdio(original_stdin, original_stdout);
//             exit (1); 
//         }
//         if (file->type == TOKEN_REDIRECT_IN) {
//             if (last_in_fd != -1) close(last_in_fd);
//             last_in_fd = open_file(file->name, 0);
//             if (last_in_fd == -1) {
//                 perror("minishell: open_file input");
//                 cleanup_stdio(original_stdin, original_stdout);
//                 return 1;
//             }
//         }
//         else if (file->type == TOKEN_HEREDOC && i == last_heredoc_index) {
//             if (last_in_fd != -1) close(last_in_fd);
//             last_in_fd = open(file->name, O_RDONLY);
//             if (last_in_fd == -1) {
//                 perror("minishell: open heredoc file");
//                 cleanup_stdio(original_stdin, original_stdout);
//                 return 1;
//             }
//         }
//         else if (file->type == TOKEN_REDIRECT_OUT) {
//             if (last_out_fd != -1) close(last_out_fd);
//             last_out_fd = open_file(file->name, 1);
//             if (last_out_fd == -1) {
//                 perror("minishell: open_file output");
//                 cleanup_stdio(original_stdin, original_stdout);
//                 return 1;
//             }
//         }
//         else if (file->type == TOKEN_APPEND) {
//             if (last_out_fd != -1) close(last_out_fd);
//             last_out_fd = open_file(file->name, 2);
//             if (last_out_fd == -1) {
//                 perror("minishell: open_file append");
//                 cleanup_stdio(original_stdin, original_stdout);
//                 return 1;
//             }
//         }
//     }
//     if (last_in_fd != -1)
//     {
//         if (dup2(last_in_fd, STDIN_FILENO) == -1)
//         {
//             perror("minishell: dup2 input");
//             close(last_in_fd);
//             cleanup_stdio(original_stdin, original_stdout);
//             return 1;
//         }
//         close(last_in_fd);
//     }
//     if (last_out_fd != -1)
//     {
//         if (dup2(last_out_fd, STDOUT_FILENO) == -1)
//         {
//             perror("minishell: dup2 output");
//             close(last_out_fd);
//             cleanup_stdio(original_stdin, original_stdout);
//             return 1;
//         }
//         close(last_out_fd);
//     }
//     close(original_stdin);
//     close(original_stdout);
//     return 0;
// }
//norminite

static int dup_original_fds(int *orig_stdin, int *orig_stdout)
{
    *orig_stdin = dup(STDIN_FILENO);
    *orig_stdout = dup(STDOUT_FILENO);
    if (*orig_stdin == -1 || *orig_stdout == -1)
    {
        perror("minishell: dup");
        return 1;
    }
    return 0;
}

static int process_input_files(t_cmd *cmd, int last_heredoc_index, int *last_in_fd)
{
    *last_in_fd = -1;
    for (int i = 0; i < cmd->file_count; i++)
    {
        t_file *file = &cmd->files[i];
        if (is_ambiguous_redirect(file->name))
        {
            ft_putstr_fd_up("minishell: ", 2);
            ft_putstr_fd_up(file->name, 2); 
            ft_putstr_fd(": ambiguous redirect\n", 2, 0);
            return (1); 
        }
        if (file->type == TOKEN_REDIRECT_IN ||
           (file->type == TOKEN_HEREDOC && i == last_heredoc_index))
        {
            if (*last_in_fd != -1)
                close(*last_in_fd);

            int fd = (file->type == TOKEN_REDIRECT_IN) ?
                     open_file(file->name, 0) : open(file->name, O_RDONLY);

            if (fd == -1)
            {
                perror("minishell: open input");
                return 1;
            }
            *last_in_fd = fd;
            if (file->type == TOKEN_HEREDOC && i == last_heredoc_index)
            {
                unlink(file->name);     // delete temp file
                free(file->name);       // prevent memory leak
                file->name = NULL;      // avoid double free
            }
        }
    }
    return 0;
}

static int process_output_files(t_cmd *cmd, int *last_out_fd)
{
    *last_out_fd = -1;
    for (int i = 0; i < cmd->file_count; i++)
    {
        t_file *file = &cmd->files[i];
        if (file->type == TOKEN_REDIRECT_OUT || file->type == TOKEN_APPEND)
        {
            if (*last_out_fd != -1)
                close(*last_out_fd);

            int mode = (file->type == TOKEN_REDIRECT_OUT) ? 1 : 2;
            int fd = open_file(file->name, mode);

            if (fd == -1)
            {
                perror("minishell: open output");
                return 1;
            }
            *last_out_fd = fd;
        }
    }
    return 0;
}

static int apply_input_redirection(int last_in_fd, int orig_stdin, int orig_stdout)
{
    if (last_in_fd == -1)
        return 0;

    if (dup2(last_in_fd, STDIN_FILENO) == -1)
    {
        perror("minishell: dup2 input");
        close(last_in_fd);
        cleanup_stdio(orig_stdin, orig_stdout);
        return 1;
    }
    close(last_in_fd);
    return 0;
}

static int apply_output_redirection(int last_out_fd, int orig_stdin, int orig_stdout)
{
    if (last_out_fd == -1)
        return 0;

    if (dup2(last_out_fd, STDOUT_FILENO) == -1)
    {
        perror("minishell: dup2 output");
        close(last_out_fd);
        cleanup_stdio(orig_stdin, orig_stdout);
        return 1;
    }
    close(last_out_fd);
    return 0;
}

int redirections(t_cmd *cmd, int last_heredoc_index)
{
    if (!cmd || cmd->file_count <= 0)
        return 0;
    int original_stdin, original_stdout;
    int last_in_fd, last_out_fd;
    if (dup_original_fds(&original_stdin, &original_stdout))
        return 1;

    if (process_input_files(cmd, last_heredoc_index, &last_in_fd))
    {
        cleanup_stdio(original_stdin, original_stdout);
        exit (1);
    }
    if (process_output_files(cmd, &last_out_fd))
    {
    if (last_in_fd != -1)
    {
        close(last_in_fd);
        last_in_fd = -1; 
    }
        cleanup_stdio(original_stdin, original_stdout);
        return 1;
    }
    if (apply_input_redirection(last_in_fd, original_stdin, original_stdout))
        return 1;
    if (apply_output_redirection(last_out_fd, original_stdin, original_stdout))
        return 1;
    close(original_stdin);
    close(original_stdout);
    return 0;
}
