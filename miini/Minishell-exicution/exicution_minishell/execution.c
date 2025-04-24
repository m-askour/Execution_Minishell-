/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:01:55 by maskour           #+#    #+#             */
/*   Updated: 2025/04/24 12:47:59 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

static void handle_cmd_errors(char *cmd_path)
{
    if(cmd_path)
        free(cmd_path);
    perror("cmmand not found");
    exit(1);
}
static void cmd_process(t_cmd *cmd, char **env)
{
    if (!cmd || !cmd->cmd || !cmd->cmd[0])
        handle_cmd_errors(NULL);
    if (!redirections(cmd))// that's to handel the rederections 
        exit(1);
    char *cmd_path;
    cmd_path = find_path(cmd->cmd[0], env);
    // if (!cmd_path)
    //     handle_cmd_error(NULL, NULL);
    if(execve(cmd_path,cmd->cmd, env) == -1)
        handle_cmd_errors(cmd_path);
}
int execute_single_command(t_cmd **cmd, char **envp)
{
    pid_t id;
    int status;
    id = fork();
    printf("single_command\n");
    if (id == 0)
        cmd_process(*cmd,envp);
    else if (id > 0)
        waitpid(id, &status, 0);
    else
        {
            perror("fork fild");
            return (1);
        }
        return(0);
}

int exicut(t_cmd **cmd, char **env)
{
    int cmd_count = 0;
    if (!cmd || !*cmd)
        return (1);
    
    while (cmd[cmd_count])
        cmd_count++;
    if (cmd_count == 1)
        execute_single_command(cmd, env);
    // else
    //     execute_pipeline(cmd, env);
    return (0);
}