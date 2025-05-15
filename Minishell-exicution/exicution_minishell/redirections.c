/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:02:07 by maskour           #+#    #+#             */
/*   Updated: 2025/05/15 16:45:31 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

static int open_file(char  *file, int mode)
{
	int fd = 0;
	if(mode == 0)
		fd = open(file, O_RDONLY);
	else if (mode == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// else if (mode == 2)
	// 	fd = open (file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}
//read in file <
static void rederect_input (t_file *file)
{
	int fd;
	fd = open_file(file->name,0);
	if (fd == -1)
	{
		perror("minishell:open");
		exit(1);
	}
	if(dup2(fd,0) == -1)
	{
		close(fd);
		perror("minishell:dup2");
		exit(1);
	}
	close(fd);
}
// write in file >
static void rederect_output(t_file *file)
{
	int fd;
	fd = open_file(file->name,1);
	if (fd == -1)
	{
		close(fd);
		exit(1);

	}
	if(dup2(fd, 1) == -1)
	{
		close(fd);
		exit(1);
	}
	close(fd);
}
// add in file >>

static void rederect_add_to_file(t_file *file)
{
	int fd;
	fd = open_file(file->name, 2);
	if (fd == -1)
	{
		close(fd);
		exit(1);
	}
	if(dup2(fd, 1) == -1)
	{
		close(fd);
		exit(1);
	}
	close(fd);
}
static void *get_rundem_name(void)
{
    char *base = "tmp/herdoc";
    char *filename;
    int fd;
    char *count_str;
    char *tmp;

    // First attempt with base name
    filename = ft_strdup(base);
    if (!filename)
        return (NULL);

    fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0600);
    if (fd != -1)
    {
        close(fd);
        return (filename);
    }
    free(filename);

    // Try with numbered suffixes
    int count = 0;
    while (count < 1000)
    {
        count_str = ft_itoa(count);
        if (!count_str)
            return (NULL);

        filename = ft_strjoin(base, "_");
        if (!filename)
        {
            free(count_str);
            return (NULL);
        }

        tmp = ft_strjoin(filename, count_str);
        free(filename);
        free(count_str);
        
        if (!tmp)
            return (NULL);

        filename = tmp;
        fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0600);
        if (fd != -1)
        {
            close(fd);
            return (filename);
        }
        
        free(filename);
        count++;
    }
    return (NULL);
}
static void function_herdoc(t_file *file)
{
	char *filename = get_rundem_name();//THIS TO CREAT RANDEM FILE;
	if (!filename)// this for the open can't 
	{
		perror ("cant creat temporory file");
		exit(1);
	}
	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC,0644);
	if (fd == -1)// this for the open can't 
	{
		perror ("cant open the file");
		free(filename);
		exit(1);
	}
	char *line;
	printf("delemiteur is : %s\n",file->name);
	while(1)
	{
		line = readline("> ");
		if (!ft_strcmp(line, file->name))//this to desplay the herdoc
		{
			free(line);
			break;
		}
		printf("Line entered: %s\n", line);
		printf("File name: %s\n", file->name);
		write(fd, line,ft_strlen(line));
		write(fd,"\n", 1);
		free(line);
	}
	close(fd);
	fd = open(filename,O_RDONLY);
	unlink(filename);// delet the fd after opening 
	dup2(fd,0);
	close(fd);
}

void redirections(t_cmd *cmd)
{
	int i = 0;
	if (!cmd)
		return ;
	t_file *files;
	while (i < cmd->file_count)
	{
		files = &cmd->files[i];
		/// this for the input file
		if (files->type == TOKEN_REDIRECT_IN)
			rederect_input(files);
		///this for the output file
		else if (files->type == TOKEN_REDIRECT_OUT)
			rederect_output(files);
		/// this for the add for the output file or creat it if it's not exist
		else if (files->type == TOKEN_APPEND)
			rederect_add_to_file(files);
		//this for herdoc
		else if (files->type == TOKEN_HEREDOC)
			function_herdoc(files);
		i++;
	}
}