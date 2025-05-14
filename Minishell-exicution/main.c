/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:08:30 by maskour           #+#    #+#             */
/*   Updated: 2025/05/14 16:51:49 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

// this the main is just for testing
int main(int ac,char **av,char **env)
{
    (void)ac;
    (void)av;
    char        *input;
    t_token     *tokens;
    t_cmd       *commands;
    t_env *env_list = file_inv(env);
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
        commands = expand_cmd_list(commands);
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
    }
    return (0);
}

