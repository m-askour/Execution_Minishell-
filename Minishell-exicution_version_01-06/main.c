/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:08:30 by maskour           #+#    #+#             */
/*   Updated: 2025/06/22 15:54:59 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

// static void remove_env_key(t_env **env, const char *key) {
//     t_env *current = *env, *prev = NULL;
//     int key_len = strlen(key);

//     while (current) {
//         if (!strncmp(current->data_env, key, key_len) &&
//             (current->data_env[key_len] == '=' || current->data_env[key_len] == '\0')) 
//         {
//             if (prev)
//                 prev->next = current->next;
//             else
//                 *env = current->next;

//             free(current->data_env);
//             free(current);
//             return;
//         }
//         prev = current;
//         current = current->next;
//     }
// }
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
void ff(void)
{
    system("leaks minishell");
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
    signal(SIGINT, handler_sig); 
    signal(SIGQUIT, handler_sig); 
    t_shell *shell_ctx;
    shell_ctx = malloc (sizeof(t_shell));
    if(!shell_ctx)
        exit(1);
    shell_ctx->exit_status = 0; 
    while (1)
    {
        atexit(ff);
        input = readline("minishell$ ");
        if (!input)
        {            
            write(1 ,"exit\n", 5);
            break ;
        }
        if (*input)
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
        if (!commands)
        {
            free_tokens(tokens, input);
            continue ;
        }
        commands = expand_cmd_list(commands, shell_ctx, env_table);
        if (!commands)
        {
            free_cmd_list(commands);
            free_tokens(tokens, input);
            continue ;
        }
        exicut(&commands, &env_list, shell_ctx);
        if(commands)
            free_cmd_list(commands);
        free_tokens(tokens, input);
        free_char_array(env_table);
    }
    // remove_env_key(&env_list, "OLDPWD");
    free_env_list(env_list);
    free(shell_ctx);
    return (0);
}

