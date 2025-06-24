/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:02:07 by maskour           #+#    #+#             */
/*   Updated: 2025/06/24 19:18:45 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"
static int open_file(char *file, int mode)
{
    int fd = 0;
    if (mode == 0)
        fd = open(file, O_RDONLY);
    else if (mode == 1)
        fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (mode == 2)
        fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    return (fd);
}
int cleanup_stdio(int original_stdin, int original_stdout)
{
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
    return 1;
}

int redirections(t_cmd *cmd)
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
    int last_heredoc_index = -1;
    int i = -1;

    // Handle heredocs
    while (++i < cmd->file_count)
    {
        if (cmd->files[i].type == TOKEN_HEREDOC) {
            if (function_herdoc(&cmd->files[i]) != 0)
                return cleanup_stdio(original_stdin, original_stdout);
            last_heredoc_index = i;
        }
    }
    i = -1;
    while (++i < cmd->file_count)
    {
        t_file *file = &cmd->files[i];
        if (file->type == TOKEN_REDIRECT_IN) {
            if (last_in_fd != -1) close(last_in_fd);
            last_in_fd = open_file(file->name, 0);
            if (last_in_fd == -1) {
                perror("minishell1");
                return cleanup_stdio(original_stdin, original_stdout);
            }
        }
        else if (file->type == TOKEN_HEREDOC && i == last_heredoc_index) {
            if (last_in_fd != -1) close(last_in_fd);
            last_in_fd = open(file->name, O_RDONLY);
            if (last_in_fd == -1)
                return cleanup_stdio(original_stdin, original_stdout);
        }
        else if (file->type == TOKEN_REDIRECT_OUT) {
            if (last_out_fd != -1) close(last_out_fd);
            last_out_fd = open_file(file->name, 1);
            if (last_out_fd == -1) {
                perror("minishell");
                return cleanup_stdio(original_stdin, original_stdout);
            }
        }
        else if (file->type == TOKEN_APPEND) {
            if (last_out_fd != -1) close(last_out_fd);
            last_out_fd = open_file(file->name, 2);
            if (last_out_fd == -1) {
                perror("minishell");
                return cleanup_stdio(original_stdin, original_stdout);
            }
        }
    }
    if (last_in_fd != -1)
    {
        if (dup2(last_in_fd, STDIN_FILENO) == -1)
        {
            perror("minishell");
            close(last_in_fd);
            return cleanup_stdio(original_stdin, original_stdout);
        }
        close(last_in_fd);
    }
    if (last_out_fd != -1)
    {
        if (dup2(last_out_fd, STDOUT_FILENO) == -1)
        {
            perror("minishell");
            close(last_out_fd);
            return cleanup_stdio(original_stdin, original_stdout);
        }
        close(last_out_fd);
    }
    close(original_stdin);
    close(original_stdout);
    return 0;
}
