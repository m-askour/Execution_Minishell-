/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exicution_pipex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 16:50:41 by maskour           #+#    #+#             */
/*   Updated: 2025/04/24 15:28:11 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
static int	handle_fork_error(int *pipfd)
{
	perror("Fork Error");
	close(pipfd[0]);
	close(pipfd[1]);
	return (1);
}

static void	handle_cmd_errors(char **cmd_arg, char *cmd_path)
{
	close(0);
	close(1);
	if (cmd_arg)
		free_split(cmd_arg);
	if (cmd_path)
		free(cmd_path);
	perror("command not found");
	exit(1);
}

void	first_cmd_process(char **str, int *pipfd, char **env)
{
	char	**cmd_arg;
	char	*cmd_path;
	int		fd;

	fd = open_file(str[1], 0);
	if (fd == -1)
	{
		close(pipfd[1]);
		close(pipfd[0]);
		perror("open failed");
		exit(1);
	}
	dup2(fd, 0);
	close(fd);
	dup2(pipfd[1], 1);
	close(pipfd[1]);
	close(pipfd[0]);
	cmd_arg = splite_cmd(str[2]);
	if (!cmd_arg)
		handle_cmd_errors(NULL, NULL);
	cmd_path = find_path(cmd_arg[0], env);
	if (!cmd_path)
		handle_cmd_errors(cmd_arg, NULL);
	if (execve(cmd_path, cmd_arg, env) == -1)
		handle_cmd_errors(cmd_arg, cmd_path);
}

void	second_cmd_process(char **str, int *pipfd, char **env)
{
	char	**cmd_arg;
	char	*cmd_path;
	int		fd;

	fd = open_file(str[4], 1);
	if (fd == -1)
	{
		close(pipfd[1]);
		close(pipfd[0]);
		perror("open filed");
		exit(1);
	}
	dup2(pipfd[0], 0);
	close(pipfd[0]);
	dup2(fd, 1);
	close(fd);
	close(pipfd[1]);
	cmd_arg = splite_cmd(str[3]);
	if (!cmd_arg)
		handle_cmd_errors(NULL, NULL);
	cmd_path = find_path(cmd_arg[0], env);
	if (!cmd_path)
		handle_cmd_errors(cmd_arg, NULL);
	if (execve(cmd_path, cmd_arg, env) == -1)
		handle_cmd_errors(cmd_arg, cmd_path);
}

int	main(int ac, char **av, char **envp)
{
	int		pipfd[2];
	int		status;
	pid_t	id1;
	pid_t	id2;

	if (ac != 5)
		return (write(2, \
			"Error: Invalid number of arguments\n", 34), 1);
	if (pipe(pipfd) == -1)
		return (perror("Pipe error"), 1);
	id1 = fork();
	if (id1 == -1)
		return (handle_fork_error(pipfd));
	if (id1 == 0)
		first_cmd_process(av, pipfd, envp);
	id2 = fork();
	if (id2 == -1)
		return (handle_fork_error(pipfd));
	if (id2 == 0)
		second_cmd_process(av, pipfd, envp);
	(close(pipfd[0]), close(pipfd[1]));
	waitpid(id2, &status, 0);
	while (wait(NULL) == -1)
		;
	return (WEXITSTATUS(status));
}
#include "../../minishell.h"

static void	handle_cmd_errors(char **cmd_arg, char *cmd_path)
{
	if (cmd_arg)
		free_split(cmd_arg);
	if (cmd_path)
		free(cmd_path);
	perror("command not found");
	exit(1);
}

static void	exec_cmd(char *cmd_str, char **env)
{
	char	**cmd_arg;
	char	*cmd_path;

	cmd_arg = splite_cmd(cmd_str);
	if (!cmd_arg)
		handle_cmd_errors(NULL, NULL);
	cmd_path = find_path(cmd_arg[0], env);
	if (!cmd_path)
		handle_cmd_errors(cmd_arg, NULL);
	if (execve(cmd_path, cmd_arg, env) == -1)
		handle_cmd_errors(cmd_arg, cmd_path);
}

static void	pipe_loop(char **cmds, int cmd_count, char **env, int infile, int outfile)
{
	int		i;
	int		pipfd[2];
	pid_t	pid;
	int		prev_pipe_in = -1;

	i = -1;
	while (++i < cmd_count)
	{
		if (i < cmd_count - 1 && pipe(pipfd) == -1)
			{
				perror("pipe failed");
				exit(1);
			}
		pid = fork();
		if (pid == -1)
			{
			perror("fork failed");	
			exit(1);
			}
		if (pid == 0)
		{
			if (i == 0) // First command: read from infile
				dup2(infile, STDIN_FILENO);
			else // Middle commands: read from previous pipe
				dup2(prev_pipe_in, STDIN_FILENO);
			if (i == cmd_count - 1) // Last command: write to outfile
				dup2(outfile, STDOUT_FILENO);
			else // Middle commands: write to next pipe
				dup2(pipfd[1], STDOUT_FILENO);
			close(pipfd[0]);
			close(pipfd[1]);
			exec_cmd(cmds[i], env);
		}
		if (prev_pipe_in != -1)
			close(prev_pipe_in);
		if (i < cmd_count - 1)
			prev_pipe_in = pipfd[0];
		close(pipfd[1]);
	}
	close(infile);
	close(outfile);
	while (wait(NULL) != -1)
		;
}

int	execute_pipeline(int ac, char **av, char **env)
{
	int	infile;
	int	outfile;
	int	cmd_count;

	if (ac < 5)
		return (write(2, "Usage: ./pipex <infile> <cmd1> <cmd2> ... <outfile>\n", 50), 1);
	cmd_count = ac - 3;
	infile = open_file(av[1], 0);
	if (infile == -1)
		perror("infile open failed");
	outfile = open_file(av[ac - 1], 1);
	if (outfile == -1)
		perror("outfile open failed");
	pipe_loop(&av[2], cmd_count, env, infile, outfile);
	return (0);
}
