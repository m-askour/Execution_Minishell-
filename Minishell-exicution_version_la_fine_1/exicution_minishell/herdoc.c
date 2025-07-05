/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:00:28 by maskour           #+#    #+#             */
/*   Updated: 2025/06/24 19:20:31 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char *rundem_name(char *file_name)
{
    char *count_str;
    char *tmp;
    int count;
    int fd;
    int count;
    char *filename;
    
    count = 0;
    while (count < 1000)
    {
    count_str = ft_itoa(count);
    if (!count_str) return NULL;
    filename = ft_strjoin(file_name, "_");
    if (!filename) { free(count_str); return NULL; }
    tmp = ft_strjoin(filename, count_str);
    free(filename);
    free(count_str);
    if (!tmp) return NULL;
    filename = tmp;
    fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0600);
    if (fd != -1) { close(fd); return filename; }
    free(filename);
    count++;
    }
    return (NULL);
}

static char *get_rundem_name(char *file_name)
{
    char *filename;
    int fd;

    filename = ft_strdup(file_name);
    if (!filename) return NULL;
    fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0600);
    if (fd != -1) { close(fd); return filename; }
    free(filename);
    return (rundem_name(file_name));
}

int function_herdoc(t_file *file)
{
    char *filename = get_rundem_name(file->name);
    if (!filename) { perror("minishell: cannot create temporary file"); return 1;}
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) { perror("minishell: open"); free(filename); return 1; }
    char *line;
    printf("delimiter is : %s\n", file->name);
    while (1) {
        line = readline("> ");
        if (!line) break;
        if (!ft_strcmp(line, file->name)) { free(line); break; }
        write(fd, line, strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);
    free(file->name);
    file->name = filename;
    signal(SIGINT, handler_sig);
    signal(SIGQUIT, handler_sig);
    return 0;
}
