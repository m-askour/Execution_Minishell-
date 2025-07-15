/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:23:33 by maskour           #+#    #+#             */
/*   Updated: 2025/07/13 14:26:57 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char *build_tmp_path(char *prefix, char *file_name)
{
    char *tmp = ft_strjoin(prefix, file_name);
    return tmp;
}

static int try_create_file(char *filename)
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0600);
    if (fd != -1)
        close(fd);
    return fd;
}

static char *build_numbered_name(char *file_name, int count)
{
    char *count_str = ft_itoa(count);
    if (!count_str)
        return NULL;

    char *tmp = ft_strjoin(file_name, "_");
    if (!tmp)
    {
        free(count_str);
        return NULL;
    }

    char *numbered = ft_strjoin(tmp, count_str);
    free(tmp);
    free(count_str);
    return numbered;
}

static char *build_full_numbered_path(char *numbered_name)
{
    char *tmp = ft_strjoin("/tmp/minishell_", numbered_name);
    return tmp;
}

static char *get_rundem_name(char *file_name)
{
    char *filename = build_tmp_path("/tmp/minishell_", file_name);
    if (!filename)
        return NULL;

    if (try_create_file(filename) != -1)
        return filename;

    free(filename);
    int count = -1;
    while ( ++count < 1000)
    {
        char *numbered_name = build_numbered_name(file_name, count);
        if (!numbered_name)
            return NULL;
        char *full_path = build_full_numbered_path(numbered_name);
        free(numbered_name);
        if (!full_path)
            return NULL;
        if (try_create_file(full_path) != -1)
            return full_path;
        free(full_path);
    }
    return NULL;
}

static void heredoc_child_loop(t_file *file, char **env, t_shell *shell_ctx, int fd)
{
    while (1)
    {
        char *line = readline("> ");
        if (!line)
        {
            close(fd);
            exit(2);
        }
        if (!ft_strcmp(line, file->name))
        {
            free(line);
            break;
        }
        if (file->check_expand == 0 && ft_strcmp(line, file->name) != 0)
        {
            char *expanded_line = found_env(line, env, shell_ctx);
            if (!expanded_line)
            {
                free(line);
                close(fd);
                unlink(file->name);
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
        free(line);
    }
}

static void heredoc_child_process(t_file *file, char **env, t_shell *shell_ctx, char *filename)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("minishell: open");
        free(filename);
        exit(1);
    }

    heredoc_child_loop(file, env, shell_ctx, fd);

    close(fd);
    free(filename);
    exit(0);
}

static int heredoc_handle_status(int status, t_file *file, char *filename)
{
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
        unlink(filename);
        free(filename);
        return 130;
    }
    if (WIFEXITED(status) && WEXITSTATUS(status) == 2)
    {
        free(file->name);
        file->name = filename;
        return 2;
    }
    else if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
    {
        free(file->name);
        file->name = filename;
        return 0;
    }
    unlink(filename);
    free(filename);
    return 0;
}

int function_herdoc(t_file *file, char **env, t_shell *shell_ctx)
{
    char *filename = get_rundem_name(file->name);
    if (!filename)
    {
        perror("minishell: cannot create temp file");
        free(filename);
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
        heredoc_child_process(file, env, shell_ctx, filename);

    int status;
    waitpid(pid, &status, 0);

    return heredoc_handle_status(status, file, filename);
}
