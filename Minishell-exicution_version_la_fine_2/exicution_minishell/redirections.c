/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:02:07 by maskour           #+#    #+#             */
/*   Updated: 2025/07/06 19:52:29 by maskour          ###   ########.fr       */
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
static char *get_rundem_name(char *delimiter)
{
    char *filename;

    filename = ft_strjoin("/tmp/", delimiter);
    if (!filename)
        return NULL;

    // Optionally, you can check if the file exists, and unlink it to start fresh
    unlink(filename);  // remove existing file if any, ignore errors

    return filename;
}
static void cleanup_fds(int original_stdin, int original_stdout, int last_in_fd, int last_out_fd)
{
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
    if (last_in_fd != -1)
        close(last_in_fd);
    if (last_out_fd != -1)
        close(last_out_fd);
}

int function_herdoc(t_file *file, char **env, t_shell *shell_ctx){
    char *filename = get_rundem_name(file->name);
    if (!filename)
    {
        perror("minishell: cannot create temp file");
        return 1;
    }
            int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("minishell: open");
        free(filename);
        exit(1);
    }
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("minishell: fork");
        free(filename);
        return 1;
    }

    signal(SIGINT, SIG_IGN);
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);



        char *line;
        while (1)
        {
            line = readline("> ");
            if (!line) {
                break; // <- Important: exit with 0 on Ctrl+D
            }   
            if (!ft_strcmp(line, file->name))  // Note: file->name still points to original delimiter in parent; you might want to pass delimiter separately or keep original delimiter
            {
                free(line);
                break;
            }

            // your expansion logic here...
            char *tmp = ft_strjoin("$",file->name);
            if (file->check_expand == 0 && ft_strcmp(line, tmp) != 0)
            {
                printf("dkhal hna\n");
                char *expanded_line = found_env(line, env, shell_ctx);
                if (!expanded_line)
                {
                    free(line);
                    close(fd);
                    unlink(filename);
                    exit (1);
                }
                write(fd, expanded_line, ft_strlen(expanded_line));
                write(fd, "\n", 1);
                free(expanded_line);
            }
            else 
            {
                write(fd, line, strlen(line));
                write(fd, "\n", 1);
                free(line);
            }
        }

        close(fd);
        free(filename);
        exit(0);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);

        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
            return 1;

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            return 1;

        // update parent file->name to point to the temp file
        free(file->name);
        file->name = filename;

        return 0;
    }
}

// ----- Redirection logic -----
int redirections(t_cmd *cmd, char **env, t_shell *shell)
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
    int i;
    int last_heredoc_idx = -1;
    i = -1;
    while (++i < cmd->file_count) {
        if (cmd->files[i].type == TOKEN_HEREDOC)
            last_heredoc_idx = i;
    }
    if (last_heredoc_idx != -1) 
    {

        if (function_herdoc(&cmd->files[last_heredoc_idx],env, shell)) {
            cleanup_fds(original_stdin, original_stdout, last_in_fd, last_out_fd);
            return 1;
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
            perror("minishell: open input file");
            cleanup_fds(original_stdin, original_stdout, last_in_fd, last_out_fd);
            return 1;
            }
        }
        else if (file->type == TOKEN_HEREDOC) {
            if (last_in_fd != -1) close(last_in_fd);
            last_in_fd = open(file->name, O_RDONLY);
            if (last_in_fd == -1) 
            {
                perror("minishell: open heredoc file");
               cleanup_fds(original_stdin, original_stdout, last_in_fd, last_out_fd);
                return (1);
            }
        }
        // Output redirections: only open, keep the last one
        else if (file->type == TOKEN_REDIRECT_OUT) {
            if (last_out_fd != -1) close(last_out_fd);
            last_out_fd = open_file(file->name, 1);
            if (last_out_fd == -1) {
            perror("minishell: open output file");
            cleanup_fds(original_stdin, original_stdout, last_in_fd, last_out_fd);
            return (1);
            }
        }
        else if (file->type == TOKEN_APPEND) {
            if (last_out_fd != -1) close(last_out_fd);
            last_out_fd = open_file(file->name, 2);
            if (last_out_fd == -1) {
            perror("minishell: open append file");
            cleanup_fds(original_stdin, original_stdout, last_in_fd, last_out_fd);
            return (1);
            }
        }
    }
    if (last_in_fd != -1) {
        if (dup2(last_in_fd, STDIN_FILENO) == -1) {
            perror("minishell: dup2 input");
            close(last_in_fd);
            cleanup_fds(original_stdin, original_stdout, -1, last_out_fd);
            return (1);
        }
        close(last_in_fd);
    }
    if (last_out_fd != -1) {
        if (dup2(last_out_fd, STDOUT_FILENO) == -1) {
            perror("minishell: dup2 output");
            close(last_out_fd);
            cleanup_fds(original_stdin, original_stdout, -1, -1);
            return (1);
        }
        close(last_out_fd);
    }

    close(original_stdin);
    close(original_stdout);
    return 0; // Success

}
