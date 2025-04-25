/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:01:55 by maskour           #+#    #+#             */
/*   Updated: 2025/04/25 19:07:06 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"
// static char	**splite_cmd(char *str)
// {
// 	char	**splt;

// 	splt = ft_split_up(str, ' ');
// 	if (!splt)
// 		return (NULL);
// 	return (splt);
// }
static void	ft_putstr_fd_2(char *s, int fd)
{
	size_t	len;

	if (!s || fd < 0)
		return ;
	len = 0;
	while (s[len])
		len++;
	write(fd, s, len);
}
static void handle_cmd_errors(char *cmd_path)
{
    if(cmd_path)
	{
	 ft_putstr_fd_2("minishell: ", 2);
        ft_putstr_fd_2(cmd_path, 2);
        ft_putstr_fd_2(": execution failed\n", 2);
        free(cmd_path);
	}
	else
	    ft_putstr_fd_2("minishell: command error\n", 2);
    exit(1);
}
static void cmd_process(t_cmd *cmd, char **env)
{
    char *cmd_path;
    // char **cmd_arg;
    // char **cmd_arg;
    if (!cmd || !cmd->cmd || !cmd->cmd[0])
        handle_cmd_errors(NULL);
    redirections(cmd); // that's to handel the rederections
    // cmd_arg = split_cmd(cmd);
    cmd_path = find_path(cmd->cmd[0], env);
    if (!cmd_path)
    {
		ft_putstr_fd_2("minishell: ", 2);
    	ft_putstr_fd_2(cmd->cmd[0], 2);
    	ft_putstr_fd_2(": command not found\n", 2);
	}
    if(execve(cmd_path,cmd->cmd, env) == -1)
        handle_cmd_errors(cmd_path);
}
static void execute_single_command(t_cmd **cmd, char **envp)
{
    pid_t id;
    int status;
    id = fork();
    // printf("single_command\n");
    if (id == 0)
        cmd_process(*cmd,envp);
    else if (id > 0)
        waitpid(id, &status, 0);
    else
        {
            perror("fork fild");
            return ;
        }
}
static void execute_pipeline(t_cmd **cmds, int cmd_count, char **env)
{
    int pipes[2];
    pid_t pid;
    int prev_pipe = -1;
    int i = 0;
     while (i< cmd_count)
      {
        if (i < cmd_count - 1 && pipe(pipes) == -1) {
            perror("minishell: pipe");
            exit(1);
        }

        pid = fork();
        if (pid == 0) {
            // Child process
            if (i > 0) {
                dup2(prev_pipe, STDIN_FILENO);
                close(prev_pipe);
            }
            if (i < cmd_count - 1) {
                dup2(pipes[1], STDOUT_FILENO);
                close(pipes[1]);
            }
            
            // Apply command-specific redirections
            redirections(cmds[i]);
            
            // Execute command
            char *path = find_path(cmds[i]->cmd[0], env);
            if (!path) {
                ft_putstr_fd_2("minishell: ", 2);
                ft_putstr_fd_2(cmds[i]->cmd[0], 2);
                ft_putstr_fd_2(": command not found\n", 2);
                exit(127);
            }
            execve(path, cmds[i]->cmd, env);
            exit(126);
        }
        else if (pid > 0) {
            // Parent process
            if (i > 0) close(prev_pipe);
            if (i < cmd_count - 1) {
                prev_pipe = pipes[0];
                close(pipes[1]);
            }
        }
        else {
            perror("minishell: fork");
            exit(1);
        }
        i++;
    }
    
    // Cleanup
    if (prev_pipe != -1) close(prev_pipe);
    while (wait(NULL) > 0);
}
// static void	execute_pipeline(t_cmd **cmds, int cmd_count, char **env)
// {

// 	int		i;
// 	int		pipfd[2];
// 	pid_t	pid;
// 	int		prev_pipe_in = -1;

// 	i = -1;
// 	while (++i < cmd_count)
// 	{
// 		if (i < cmd_count - 1 && pipe(pipfd) == -1)
// 			{
// 				perror("pipe failed");
// 				exit(1);
// 			}
// 		pid = fork();
// 		if (pid == -1)
// 			{
// 			perror("fork failed");	
// 			exit(1);
// 			}
// 		if (pid == 0)
// 		{
// 			if (i == 0) // First command: read from infile
// 				dup2(infile, 0);
// 			else // Middle commands: read from previous pipe
// 				dup2(prev_pipe_in, 0);
// 			if (i == cmd_count - 1) // Last command: write to outfile
// 				dup2(outfile, 1);
// 			else // Middle commands: write to next pipe
// 				dup2(pipfd[1], 1);
// 			close(pipfd[0]);
// 			close(pipfd[1]);
// 			cmd_process(cmds[i], env);
// 		}
// 		if (prev_pipe_in != -1)
// 			close(prev_pipe_in);
// 		if (i < cmd_count - 1)
// 			prev_pipe_in = pipfd[0];
// 		close(pipfd[1]);
// 	}
// 	close(infile);
// 	close(outfile);
// 	while (wait(NULL) != -1)
// 		;
// }
int exicut(t_cmd **cmd, char **env)
{
    int cmd_count = 0;
    if (!cmd || !*cmd)
        return (1);
    
    while (cmd[cmd_count])
        cmd_count++;
    if (cmd_count == 1)
        execute_single_command(cmd, env);
    else
        execute_pipeline(cmd, cmd_count,env);
    return (0);
}