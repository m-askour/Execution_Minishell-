/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:08:30 by maskour           #+#    #+#             */
/*   Updated: 2025/06/28 19:36:34 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

void free_char_array(char **array)
{
    int i;

    if (!array)
        return;
    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}
void ff()
{
    system("leaks minishell");
}
static void sign(t_shell *shell)
{
    signal(SIGINT, handler_sig);
    shell->exit_status = 1;
    
}
int main(int ac,char **av,char **env)
{
    (void)ac;
    (void)av;
    char        *input;
    t_token     *tokens;
     t_cmd       *commands;
    t_env *env_list;
    env_list = file_inv(env);
    t_shell *shell_ctx;
    shell_ctx = malloc (sizeof(t_shell));
    if(!shell_ctx)
        exit(1);
    shell_ctx->exit_status = 0; 
    sign(shell_ctx);
    signal(SIGQUIT, handler_sig); 
    while (1)
    {
        // atexit(ff);
        input = readline("minishell$ ");
        if (!input)
        {     
            write(1 ,"exit\n", 5);
            free_env_list(env_list);
            // free(shell_ctx);
            exit(shell_ctx->exit_status) ;
        }
        if (input[0])
            add_history(input);
        char **env_table = convert(env_list);
        tokens = check_quoted(input, shell_ctx, env_table);
        if (!tokens)
        {
            free (input);
            free_char_array(env_table);
            continue ;
        }
        commands = parse_commands(tokens , shell_ctx);
        print_command_with_files(commands);
        if (!commands)
        {
            free_tokens(tokens, input);
            continue ;
        }
        free_char_array(env_table);
        exicut(&commands, &env_list, shell_ctx);
        if(commands)
            free_cmd_list(commands);
        free_tokens(tokens, input);
    }
    free_env_list(env_list);
    free(shell_ctx);
    write(1 ,"exit\n", 5);
    return (0);
}

