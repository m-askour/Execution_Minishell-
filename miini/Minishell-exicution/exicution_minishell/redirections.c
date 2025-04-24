/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:02:07 by maskour           #+#    #+#             */
/*   Updated: 2025/04/23 19:21:22 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

static int open_file(char  *file, int mode)
{
	int fd;
	if(mode == 0)
		fd = open(file, O_RDONLY);
	else if (mode == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (mode == 2)
		fd = open (file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}
//read in file <
void rederect_input (char *arg)
{
	int fd;
	fd = open_file(arg,0);
	if (fd == -1)
	{
		close(fd);
		exit(1);
	}
	if(!dup2(fd,0) == -1)
	{
		close(fd);
		exit(1);
	}
}
// write in file >
void rederect_output(char *arg)
{
	int fd;
	fd = open_file(arg,1);
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
}
// add in file >>

void rederect_add_to_file(char *arg)
{
	int fd;
	fd = open_file(arg, 2);
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
}
char *get_rundem_name()
{
	char *base = "tmp/herdoc";
	char *filename;
	int fd;
	char *count_str;
	char *tmp;
	filename = ft_strdup(base);
	if (!filename)
		exit(1);
	fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0600);
	if (fd != -1)
	{
		close (fd);
		return (filename);
	}
	free(filename);
	int count = 0;
	while (count < INT_MAX)
	{
		count_str = ft_itoi(count);
		if (!count_str)
			exit(1);
		filename = ft_strjoin(base, "_");
		tmp = ft_strjoin(filename,count_str);
		free(filename);
		free(count_str);
		filename = tmp;
		fd = open (filename, O_WRONLY| O_CREAT | O_EXCL, 0600);
		if (fd != -1)
		{
			close (fd);
			return(filename);
		}
		free(filename);
		count++;
	}
	perror("filed to creat unique herdoc filww \n");
	exit(1);
}
void function_herdoc(char *arg)
{
	char *filename = get_rundem_name();
	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC,0644);
	if (fd == -1)// this for the open can't 
	{
		perror ("cant open the file");
		exit(1);
	}
	char *line;
	while(1)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, arg) == 0)
			break;
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
int redirections(t_cmd *cmd, char **env)
{
	int i = 0;
	int fd;
	if (!cmd)
		return (0);
	t_file *files;
	while (i < cmd->file_count)
	{
		files = &cmd->files[i];
		/// this for the input file
		if (files->type == TOKEN_REDIRECT_IN)
		{
			rederect_input(files);
		}
		///this for the output file
		else if (files->type == TOKEN_REDIRECT_OUT)
		{
			rederect_output(files);
			i++;
		}
		/// this for the add for the output file or creat it if it's not exist
		else if (files->type == TOKEN_APPEND)
		{
			rederect_add_to_file(files);
			i++;
		}
		//this for herdoc
		else if (files->type == TOKEN_HEREDOC)
		{
			function_herdoc(files);
			i++;
		}
		i++;
	}
}