/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:08:30 by maskour           #+#    #+#             */
/*   Updated: 2025/05/24 20:39:00 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// this main for marge
int main(int ac,char **av,char **env)
{
    (void)ac;
    (void)av;
    char        *input;
    t_token     *tokens;
    t_cmd       *commands;
    t_env *env_list;
    
    env_list = file_inv(env);
    while (1)
    {
        input = readline("minishell$ ");
        if (!input)
        {
            printf("exit\n");
            break ;
        }
        if (*input)
            add_history(input);
        tokens = check_quoted(input);
        if (!tokens)
            continue ;
        commands = parse_commands(tokens);
        if (!commands)
        {
            free_tokens(tokens, input);
            continue ;
        }
        commands = expand_cmd_list(commands);
        if (!commands)
        {
            free_cmd_list(commands);
            free_tokens(tokens, input);
            continue ;
        }
        commands = unquote_cmd_list(commands);
        if (!commands)
        {
            free_cmd_list(commands);
            free_tokens(tokens, input);
            continue ;
        }
        if (commands)
        {
            t_cmd *current = commands;
            int i = 1;
            
            while (current)
            {
                printf("Command #%d:\n", i);
                print_command_with_files(current);
                current = current->next;
                i++;
            }
        }
        exicut(&commands, env_list);
        free_tokens(tokens, input);
        free_cmd(commands);
    }    
    // free_env_list(env_list);
    free_env_list(env_list);
    return (0);
}

