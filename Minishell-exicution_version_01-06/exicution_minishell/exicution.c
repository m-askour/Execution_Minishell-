/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exicution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:01:55 by maskour           #+#    #+#             */
/*   Updated: 2025/04/20 17:01:56 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

static void handle_cmd_errors(char **cmd_arg, char **cmd_path)
{
    if(cmd_path)
        free(cmd_path);
    perror("cmmand not found");
    exit(1);
}
static void cmd_process(char **str, char **env)
{
    if (!str || !str[0])
        handle_cmd_errors(NULL,NULL);
    char *cmd_path;
    cmd_path = find_path(str[0], env);
    if (!cmd_path)
        handle_cmd_error(str[0],NULL);
    if(execve(cmd_path,str[0], env) == -1)
        handle_cmd_error(str[0],cmd_path);
}
int main (int ac,char **av, char **env)
{
    pid_t id;
    int status;
    id = fork();
    if (id == 0)
        cmd_process(av,env);
    else if (id > 0)
        waitpid(id, &status, 0);
    else
        {
            perror("fork fild");
            return (1);
        }
        return(0);
}