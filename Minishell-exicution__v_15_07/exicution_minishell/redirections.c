/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:02:07 by maskour           #+#    #+#             */
/*   Updated: 2025/07/15 19:24:59 by maskour          ###   ########.fr       */
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
    int i;

    i = -1;
    while (++i < cmd->file_count)
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
            int fd;
            if (file->type == TOKEN_REDIRECT_IN) 
            
                fd = open_file(file->name, 0);
            else
                fd = open(file->name, O_RDONLY);

            if (fd == -1)
            {
                perror("minishell: open input");
                return 1;
            }
            *last_in_fd = fd;
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
                ft_putstr_fd_up("minishell: ", 2);
                ft_putstr_fd_up(file->name, 2);
                ft_putstr_fd(": No such file or directory\n", 2, 0);
                return 1;
            }
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
        return (2);
    }
    if (process_output_files(cmd, &last_out_fd))
    {
        if (last_in_fd != -1)
        {
            close(last_in_fd);
            last_in_fd = -1; 
        }
        cleanup_stdio(original_stdin, original_stdout);
        return 2;
    }
    if (apply_input_redirection(last_in_fd, original_stdin, original_stdout))
        return 1;
    if (apply_output_redirection(last_out_fd, original_stdin, original_stdout))
        return 1;
    close(original_stdin);
    close(original_stdout);
    return 0;
}
